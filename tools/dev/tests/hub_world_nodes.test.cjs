"use strict";
// Mocked hub API: no real processes, commands, or databases are touched.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { chromium } = require("playwright");
const web = path.resolve(__dirname, "../../../src/server/hub/web");
(async () => {
    const browser = await chromium.launch({ headless: true, ...(process.env.HUB_TEST_BROWSER_CHANNEL ? { channel: process.env.HUB_TEST_BROWSER_CHANNEL } : {}) });
    try {
        const page = await browser.newPage({ viewport: { width: 1280, height: 900 } });
        const errors = [], writes = [];
        page.on("pageerror", error => errors.push(error.message));
        let admin = true;
        const world = { key: "world", name: "SkyFire PvE", isWorld: true, managed: true, enabled: true,
            status: "online", state: "running", canSendCommands: true, uptimeSeconds: 7260, metricsAvailable: true,
            players: 12, updateTimeMs: 7, cpuPercent: 3.25, executablePath: "worldserver", configPath: "worldserver.conf", workingDirectory: "." };
        const second = { ...world, key: "world-pvp", name: "SkyFire PvP", players: 23, configPath: "world-pvp.conf", commandResult: "PvP result" };
        const components = [{ key: "authnet", name: "Authnet", managed: true, enabled: true, status: "online", state: "running", uptimeSeconds: 3660 }, world, second];
        await page.route("http://hub.test/**", async route => {
            const url = new URL(route.request().url());
            const json = (data, status = 200) => route.fulfill({ status, contentType: "application/json", body: JSON.stringify(data) });
            if (url.pathname === "/api/v1/status") return json({ username: "admin", csrfToken: "test-token", canSendWorldCommands: admin,
                canOperateServices: admin, accountsEnabled: true, uptimeSeconds: 8000, components });
            if (url.pathname.startsWith("/api/v1/services/")) {
                assert.equal(route.request().headers()["x-hub-csrf"], "test-token");
                const fields = Object.fromEntries(new URLSearchParams(route.request().postData()));
                writes.push({ path: url.pathname, fields });
                if (url.pathname.endsWith("/configure")) {
                    if (fields.configPath === "forbidden.conf") return json({ error: "Stop this world gracefully before changing its launch paths." }, 409);
                    const existing = components.find(item => item.key === fields.key);
                    if (existing) Object.assign(existing, fields);
                    else components.push({ ...world, ...fields, isWorld: true, state: "stopped", status: "offline", metricsAvailable: false });
                    return json({ saved: true });
                }
                return json({ accepted: true }, 202);
            }
            if (url.pathname.startsWith("/api/v1/accounts/")) return json({ items: [], next: 0 });
            const name = url.pathname === "/" ? "index.html" : url.pathname.slice(1);
            if (["status.js", "index.html", "app.css", "app.js", "accounts.js"].includes(name))
                return route.fulfill({ contentType: name.endsWith("js") ? "text/javascript" : name.endsWith("css") ? "text/css" : "text/html", body: fs.readFileSync(path.join(web, name)) });
            return route.fulfill({ status: 404, body: "" });
        });
        await page.goto("http://hub.test/");
        await page.locator("#world-node").waitFor({ state: "visible" });
        const cards = page.locator("#component-grid article");
        assert.match(await cards.nth(0).innerText(), /Uptime 1h 1m/);
        assert.doesNotMatch(await cards.nth(0).innerText(), /CPU|players|Update/);
        assert.match(await cards.nth(1).innerText(), /12 players\nCPU 3.3% · Update 7 ms/);
        await page.locator("#world-node").selectOption("world-pvp");
        assert.equal(await page.locator("#world-command-result").textContent(), "PvP result");
        await page.locator("#world-command").fill(".server restart 300");
        await page.locator("#world-command-send").click();
        assert.equal(writes.at(-1).path, "/api/v1/services/world-pvp/command");
        assert.equal(writes.at(-1).fields.command, ".server restart 300");
        await page.locator("#accounts-tab").click();
        assert.equal(await page.locator("#server-sidebar").isVisible(), true);
        world.state = "stopped";
        await page.evaluate(() => loadStatus());
        assert.match(await page.locator("#accounts-routing").textContent(), /executed by worldserver/);
        await cards.nth(2).getByRole("button", { name: "Edit", exact: true }).click();
        const form = page.locator("#world-node-form");
        assert.equal(await form.locator('[name="key"]').inputValue(), "world-pvp");
        await form.locator('[name="name"]').fill("PvP <script>name</script>");
        await form.getByRole("button").click();
        await page.waitForFunction(() => document.querySelector("#world-node-message").textContent === "World node saved.");
        assert.equal(await page.locator("#component-grid script").count(), 0);
        assert.match(await cards.nth(2).innerText(), /PvP <script>name<\/script>/);
        await form.locator('[name="configPath"]').fill("forbidden.conf");
        await form.getByRole("button").click();
        await page.waitForFunction(() => document.querySelector("#world-node-message").textContent.includes("Stop this world"));
        await form.locator('[name="key"]').fill("world-test");
        await form.locator('[name="name"]').fill("Test realm");
        await form.locator('[name="configPath"]').fill("test.conf");
        await form.getByRole("button").click();
        await page.waitForFunction(() => document.querySelector("#world-node-message").textContent === "World node saved.");
        await page.locator("#world-node").selectOption("world-test");
        assert.equal(await page.locator("#world-command-send").isEnabled(), false);
        second.metricsAvailable = false;
        await page.evaluate(() => loadStatus());
        assert.match(await cards.nth(2).innerText(), /Players \/ load unavailable/);
        await page.setViewportSize({ width: 390, height: 844 });
        assert.equal(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), true);
        if (process.env.HUB_TEST_SCREENSHOT) {
            await page.setViewportSize({ width: 1280, height: 900 });
            await page.evaluate(() => window.scrollTo(0, 0));
            await page.screenshot({ path: process.env.HUB_TEST_SCREENSHOT });
        }
        admin = false;
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator("#world-node-settings").isVisible(), false);
        assert.equal(await page.locator("#world-console").isVisible(), false);
        assert.equal(await page.locator("#component-grid").getByRole("button", { name: "Edit", exact: true }).count(), 0);
        assert.deepEqual(errors, []);
        console.log("Hub world node checks passed: metrics, routing, rename/create, errors, permissions, mobile layout.");
    } finally { await browser.close(); }
})().catch(error => { console.error(error); process.exitCode = 1; });
