"use strict";
// Isolated browser/API fixture. Never contacts a running hub or database.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { chromium } = require("playwright");
const web = path.resolve(__dirname, "../../../src/server/hub/web");
(async () => {
    const browser = await chromium.launch({ headless: true, ...(process.env.HUB_TEST_BROWSER_CHANNEL ? { channel: process.env.HUB_TEST_BROWSER_CHANNEL } : {}) });
    try {
        const page = await browser.newPage({ viewport: { width: 1280, height: 800 } });
        const errors = [];
        page.on("pageerror", error => errors.push(error.message));
        let count = 0, inFlight = 0, maxInFlight = 0, slow = false, fail = false, authenticated = true;
        let releaseStatus = null, releaseOperation = null;
        let registryNodes = [];
        const component = { key: "world", name: "SkyFire", managed: true, isWorld: true, enabled: true,
            status: "online", state: "running", canSendCommands: true, metricsAvailable: true, players: 4,
            cpuPercent: 1.5, updateTimeMs: 2, executablePath: "worldserver", configPath: "worldserver.conf", workingDirectory: "." };
        await page.route("http://hub.test/**", async route => {
            const url = new URL(route.request().url());
            const json = (data, status = 200) => route.fulfill({ status, contentType: "application/json", body: JSON.stringify(data) });
            if (url.pathname === "/api/v1/status") {
                ++count; ++inFlight; maxInFlight = Math.max(inFlight, maxInFlight);
                const requestCount = count;
                const data = { username: "admin", csrfToken: "token", canSendWorldCommands: true, canOperateServices: true,
                    accountsEnabled: true, uptimeSeconds: 60 + requestCount,
                    components: [{ ...component, uptimeSeconds: 60 + requestCount, players: requestCount }, ...registryNodes] };
                try {
                    if (slow) await new Promise(resolve => { releaseStatus = resolve; });
                    if (fail) return await json({ error: "temporarily unavailable" }, 503);
                    return await json(data, authenticated ? 200 : 401);
                } finally { --inFlight; }
            }
            if (url.pathname === "/api/v1/services/world/stop") {
                await new Promise(resolve => { releaseOperation = resolve; });
                return json({ accepted: true }, 202);
            }
            if (url.pathname.startsWith("/api/v1/accounts/")) return json({ items: [], next: 0 });
            if (url.pathname === "/api/v1/logout") { authenticated = false; return json({ ok: true }); }
            const name = url.pathname === "/" ? "index.html" : url.pathname.slice(1);
            if (["index.html", "app.js", "status.js", "accounts.js", "app.css"].includes(name))
                return route.fulfill({ contentType: name.endsWith("js") ? "text/javascript" : name.endsWith("css") ? "text/css" : "text/html", body: fs.readFileSync(path.join(web, name)) });
            return route.fulfill({ status: 404, body: "" });
        });
        await page.goto("http://hub.test/");
        await page.locator("#world-console").waitFor({ state: "visible" });
        await page.locator("#world-command").fill(".server restart 300");
        await page.evaluate(() => {
            window.originalCard = document.querySelector("#component-grid article");
            window.originalStop = document.querySelector("#component-grid .stop");
            window.originalOption = document.querySelector("#world-node option");
            window.originalStop.focus();
        });
        const first = await page.locator(".service-metrics").textContent();
        await page.waitForFunction(previous => document.querySelector(".service-metrics").textContent !== previous, first);
        assert.equal(await page.evaluate(() => document.activeElement === window.originalStop && window.originalCard === document.querySelector("#component-grid article") && window.originalOption === document.querySelector("#world-node option")), true);
        assert.equal(await page.locator("#world-command").inputValue(), ".server restart 300");
        await page.locator("#accounts-tab").click();
        await page.getByText("Create player account", { exact: true }).click();
        await page.locator('#account-create [name="username"]').fill("UNSAVED");
        await page.evaluate(() => window.scrollTo(0, 350));
        const scroll = await page.evaluate(() => window.scrollY);
        const beforeAccountTick = await page.locator(".service-metrics").textContent();
        await page.waitForFunction(previous => document.querySelector(".service-metrics").textContent !== previous, beforeAccountTick);
        assert.equal(await page.locator("#accounts-view").isVisible(), true);
        assert.equal(await page.locator('#account-create [name="username"]').inputValue(), "UNSAVED");
        assert.equal(await page.evaluate(() => document.activeElement === document.querySelector('#account-create [name="username"]')), true);
        assert.equal(await page.evaluate(() => window.scrollY), scroll);
        await page.locator("#status-tab").click();
        await page.locator("#component-grid .stop").click();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("Stopping"));
        const beforeOperationTick = await page.locator(".service-metrics").textContent();
        await page.waitForFunction(previous => document.querySelector(".service-metrics").textContent !== previous, beforeOperationTick);
        assert.equal(await page.locator("#component-grid .stop").isEnabled(), false, "polling must not unlock a pending action");
        releaseOperation();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("requested"));
        registryNodes = [{ key: "cluster:synthetic-world", name: "Remote world", status: "issue", detail: "Cluster world | not ready", managed: false }];
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator("#component-grid article").count(), 2);
        assert.equal(await page.locator("#component-grid article").nth(1).getByRole("button", { name: "Start", exact: true }).isVisible(), false);
        registryNodes[0].status = "online";
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator("#component-grid article").nth(1).getAttribute("class"), "component online");
        registryNodes = [];
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator("#component-grid article").count(), 1);
        assert.equal(await page.evaluate(() => window.originalStop === document.querySelector("#component-grid .stop")), true);
        slow = true;
        const refresh = page.evaluate(() => Promise.all([loadStatus(), loadStatus(), loadStatus()]));
        while (!releaseStatus) await new Promise(resolve => setTimeout(resolve, 20));
        await new Promise(resolve => setTimeout(resolve, 1200));
        assert.equal(maxInFlight, 1, "only one status request may run at once");
        slow = false; releaseStatus(); releaseStatus = null;
        await refresh;
        fail = true;
        await page.evaluate(() => loadStatus());
        assert.match(await page.locator("#last-updated").textContent(), /Connection lost/);
        assert.equal(await page.locator("#world-console").isVisible(), true);
        fail = false;
        await page.waitForFunction(() => document.querySelector("#last-updated").textContent.startsWith("Live"));
        slow = true;
        const staleRefresh = page.evaluate(() => loadStatus());
        while (!releaseStatus) await new Promise(resolve => setTimeout(resolve, 20));
        await page.locator("#logout-button").click();
        await page.locator("#login-view").waitFor({ state: "visible" });
        slow = false; releaseStatus(); releaseStatus = null;
        await staleRefresh;
        const afterLogout = count;
        await new Promise(resolve => setTimeout(resolve, 1200));
        assert.equal(count, afterLogout, "logout stops polling");
        assert.equal(await page.locator("#app-shell").isVisible(), false, "late response must not reopen authenticated UI");
        assert.deepEqual(errors, []);
        console.log("Independent status checks passed: live ticks, stable controls/focus, unsaved forms, scroll, single request, pending actions, recovery and logout.");
    } finally { await browser.close(); }
})().catch(error => { console.error(error); process.exitCode = 1; });
