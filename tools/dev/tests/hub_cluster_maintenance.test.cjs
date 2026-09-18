"use strict";
// Isolated UI fixture; never sends maintenance commands to a real server.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { chromium } = require("playwright");
const web = path.resolve(__dirname, "../../../src/server/hub/web");
(async () => {
    const browser = await chromium.launch({ headless: true, ...(process.env.HUB_TEST_BROWSER_CHANNEL ? { channel: process.env.HUB_TEST_BROWSER_CHANNEL } : {}) });
    try {
        const page = await browser.newPage({ viewport: { width: 1280, height: 800 } });
        const errors = [], actions = [];
        page.on("pageerror", error => errors.push(error.message));
        let allowed = true, failure = false, release = null, hold = false, authenticated = true;
        const node = { key: "cluster:auth-a", clusterKey: "auth-a", clusterCanAdmin: true,
            name: "Auth A", live: true, status: "online", managed: false, adminState: "enabled", hubConnections: 2,
            detail: "Cluster authnet | policy enabled | hub connections 2" };
        const world = { key: "cluster:world-a", name: "World A", clusterKey: "world-a", clusterCanAdmin: false,
            live: true, status: "online", managed: false, adminState: "enabled", detail: "realm 1" };
        await page.route("http://hub.test/**", async route => {
            const url = new URL(route.request().url());
            const json = (body, status = 200) => route.fulfill({ status, contentType: "application/json", body: JSON.stringify(body) });
            if (url.pathname === "/api/v1/status") return json({ username: "operator", csrfToken: "csrf-test",
                canOperateServices: allowed, canSendWorldCommands: true, accountsEnabled: true,
                uptimeSeconds: 200, components: [{ ...node }, world] }, authenticated ? 200 : 401);
            if (url.pathname.startsWith("/api/v1/cluster/")) {
                assert.equal(route.request().method(), "POST");
                assert.equal(route.request().headers()["x-hub-csrf"], "csrf-test");
                const action = url.pathname.split("/").pop();
                actions.push(action);
                if (hold) await new Promise(resolve => { release = resolve; });
                if (failure) return json({ error: "Policy persistence could not be verified" }, 409);
                node.adminState = { drain: "draining", disable: "disabled", enable: "enabled" }[action];
                node.status = node.live ? node.adminState === "enabled" ? "online" : "issue" : "offline";
                node.detail = `Cluster authnet | policy ${node.adminState} | hub connections ${node.hubConnections}`;
                return json({ saved: true });
            }
            if (url.pathname.startsWith("/api/v1/accounts/")) return json({ items: [], next: 0 });
            if (url.pathname === "/api/v1/logout") { authenticated = false; return json({ ok: true }); }
            const file = url.pathname === "/" ? "index.html" : url.pathname.slice(1);
            if (["index.html", "app.js", "status.js", "accounts.js", "app.css"].includes(file))
                return route.fulfill({ contentType: file.endsWith("js") ? "text/javascript" : file.endsWith("css") ? "text/css" : "text/html", body: fs.readFileSync(path.join(web, file)) });
            return route.fulfill({ status: 404, body: "" });
        });
        await page.goto("http://hub.test/");
        const auth = page.locator("#component-grid article").nth(0);
        const realm = page.locator("#component-grid article").nth(1);
        await auth.getByRole("button", { name: "Drain", exact: true }).waitFor();
        assert.equal(await auth.locator(".start").isVisible(), false);
        assert.equal(await auth.locator(".enable").isEnabled(), false);
        assert.equal(await realm.locator(".drain").isVisible(), false);
        await page.locator("#accounts-tab").click();
        await page.getByText("Create player account", { exact: true }).click();
        await page.locator('#account-create [name="username"]').fill("KEEP-MY-FORM");
        await auth.evaluate(el => { window.originalClusterCard = el; });
        hold = true;
        await auth.locator(".drain").click();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("Draining"));
        await page.evaluate(() => loadStatus());
        assert.equal(await auth.locator(".disable").isEnabled(), false, "polling unlocked a pending maintenance operation");
        while (!release) await new Promise(resolve => setTimeout(resolve, 10));
        hold = false; release();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("Drain saved"));
        await page.evaluate(() => loadStatus());
        assert.equal(await auth.locator(".status-label").textContent(), "draining");
        assert.equal(await auth.locator(".drain").isEnabled(), false);
        node.hubConnections = 0; node.detail = "Cluster authnet | policy draining | hub connections 0";
        await page.evaluate(() => loadStatus());
        assert.ok((await auth.textContent()).includes("hub connections 0"));
        assert.equal(await auth.evaluate(el => window.originalClusterCard === el), true);
        assert.equal(await page.locator('#account-create [name="username"]').inputValue(), "KEEP-MY-FORM");
        node.live = false; node.status = "offline";
        await page.evaluate(() => loadStatus());
        assert.equal(await auth.locator(".enable").isEnabled(), true, "offline saved policy cannot be re-enabled");
        await auth.locator(".disable").click();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("Disable saved"));
        failure = true;
        await auth.locator(".enable").click();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("persistence"));
        assert.equal(node.adminState, "disabled");
        failure = false;
        await auth.locator(".enable").click();
        await page.waitForFunction(() => document.querySelector("#service-action-message").textContent.includes("Enable saved"));
        allowed = false; await page.evaluate(() => loadStatus());
        assert.equal(await auth.locator(".drain").isEnabled(), false);
        assert.equal(await auth.locator(".disable").isEnabled(), false);
        assert.deepEqual(actions, ["drain", "disable", "enable", "enable"]);
        await page.setViewportSize({ width: 390, height: 844 });
        assert.equal(await page.evaluate(() => document.documentElement.scrollWidth <= window.innerWidth), true);
        assert.deepEqual(errors, []);
        console.log("Cluster maintenance UI checks passed: drain/disable/enable, CSRF, offline policy, pending actions, persistence errors, permissions, preserved forms and mobile layout.");
    } finally { await browser.close(); }
})().catch(error => { console.error(error); process.exitCode = 1; });
