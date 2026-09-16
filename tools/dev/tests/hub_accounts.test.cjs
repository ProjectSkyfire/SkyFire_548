"use strict";

// Run with Playwright and a Chromium browser installed. All HTTP is intercepted;
// this test never connects to a real hub, account database, or game server.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const { chromium } = require("playwright");
const web = path.resolve(__dirname, "../../../src/server/hub/web");

(async () => {
    const browser = await chromium.launch({ headless: true, ...(process.env.HUB_TEST_BROWSER_CHANNEL ? { channel: process.env.HUB_TEST_BROWSER_CHANNEL } : {}) });
    try {
        const page = await browser.newPage({ viewport: { width: 1280, height: 1000 } });
        const errors = [], writes = [];
        let allowed = true, enabled = true, jobError = false;
        page.on("pageerror", (error) => errors.push(error.message));
        const account = { id: 7, username: "TESTPLAYER", email: "player@example.test", expansion: 4, ip: "192.0.2.10", online: true, muteUntil: 0,
            muteReason: "", muteBy: "", banned: false, gm: [{ realm: -1, level: 0 }], rbac: [], bans: [] };
        await page.route("http://hub.test/**", async (route) => {
            const url = new URL(route.request().url());
            const form = new URLSearchParams(route.request().postData() || "");
            const json = (data, status = 200) => route.fulfill({ status, contentType: "application/json", body: JSON.stringify(data) });
            if (url.pathname === "/api/v1/status") return json({ username: "test-admin", csrfToken: "csrf-test", canOperateServices: true,
                canSendWorldCommands: allowed, accountsEnabled: enabled, uptimeSeconds: 30,
                components: [{ key: "world", name: "World", managed: true, enabled: true, status: "online", state: "running", canSendCommands: true }] });
            if (url.pathname.startsWith("/api/v1/accounts/")) {
                assert.equal(route.request().headers()["x-hub-csrf"], "csrf-test");
                const action = url.pathname.split("/").pop();
                if (action === "search") return json({ items: [{ ...account, name: account.username }], next: 0 });
                if (action === "detail") return json(account);
                if (action === "realms") return json({ items: [{ id: -1, name: "All realms" }, { id: 1, name: "Test realm" }] });
                if (action === "permissions") return json({ items: [{ id: 10, name: "<script>permission</script>" }], next: 0 });
                if (action === "ip-list") return json({ items: [], next: "" });
                if (action === "result") return jobError ? json({ error: "Worldserver is transitioning. No direct database fallback." }, 409) : json({ ok: true, id: 7, message: "Change saved by worldserver." });
                writes.push({ action, fields: Object.fromEntries(form) });
                return json({ job: "test-job" }, 202);
            }
            const name = url.pathname === "/" ? "index.html" : url.pathname.slice(1);
            if (["index.html", "app.js", "accounts.js", "app.css"].includes(name))
                return route.fulfill({ contentType: name.endsWith("js") ? "text/javascript" : name.endsWith("css") ? "text/css" : "text/html", body: fs.readFileSync(path.join(web, name)) });
            return route.fulfill({ status: 404, body: "" });
        });
        await page.goto("http://hub.test/");
        await page.locator("#accounts-tab").click();
        await page.locator("#account-results button").click();
        await page.locator("#account-profile input[name=username]").waitFor({ state: "visible" });
        assert.equal(await page.locator("#account-profile input[name=username]").inputValue(), "TESTPLAYER");
        assert.match(await page.locator("#accounts-routing").textContent(), /executed by worldserver/);
        await page.locator("#account-profile input[name=email]").fill("updated@example.test");
        await page.locator("#account-profile input[name=password]").fill("Test123!");
        await page.locator("#account-profile button").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
        assert.equal(writes[0].action, "update");
        assert.equal(writes[0].fields.id, "7");
        assert.equal(writes[0].fields.password, "Test123!");
        assert.equal(await page.locator("#account-profile input[name=password]").inputValue(), "");
        assert.equal(await page.evaluate(() => localStorage.length), 0);

        for (const [id, action] of [["account-ban", "ban"], ["account-mute", "mute"]]) {
            await page.locator(`#${id} input[name=reason]`).fill("Test reason");
            await page.locator(`#${id} button`).click();
            await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
            assert.equal(writes.at(-1).action, action);
        }
        for (const action of ["unban", "unmute"]) {
            await page.locator(`#account-${action}`).click();
            await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
            assert.equal(writes.at(-1).action, action);
        }
        await page.locator("#account-gm select[name=realm]").selectOption("1");
        await page.locator("#account-gm select[name=level]").selectOption("2");
        await page.locator("#account-gm button").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
        assert.equal(writes.at(-1).fields.realm, "1");
        assert.equal(writes.at(-1).fields.level, "2");
        await page.getByText("Find an RBAC permission", { exact: true }).click();
        await page.locator("#permission-search button").click();
        await page.locator("#permission-results button").click();
        assert.equal(await page.locator("#account-rbac input[name=permission]").inputValue(), "10");
        assert.equal(await page.locator("#permission-results script").count(), 0);
        for (const mode of ["grant", "deny", "revoke"]) {
            await page.locator("#account-rbac select[name=mode]").selectOption(mode);
            await page.locator("#account-rbac button").click();
            await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
            assert.equal(writes.at(-1).fields.mode, mode);
        }
        await page.locator("#ip-ban-form input[name=ip]").fill("192.0.2.15");
        await page.locator("#ip-ban-form input[name=reason]").fill("Test IP ban");
        await page.locator("#ip-ban-form input[name=minutes]").fill("0");
        await page.locator("#ip-ban-form button").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
        assert.equal(writes.at(-1).action, "ip-ban");
        assert.equal(writes.at(-1).fields.minutes, "0");
        await page.locator("#ip-unban").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
        assert.equal(writes.at(-1).action, "ip-unban");

        await page.getByText("Create player account", { exact: true }).click();
        for (const [name, value] of Object.entries({ username: "NEWPLAYER", email: "new@example.test", password: "Test123!" }))
            await page.locator(`#account-create input[name=${name}]`).fill(value);
        await page.locator("#account-create button").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("Change saved"));
        assert.equal(writes.at(-1).action, "create");
        assert.equal(await page.locator("#account-create input[name=password]").inputValue(), "");
        jobError = true;
        await page.locator("#account-unban").click();
        await page.waitForFunction(() => document.querySelector("#accounts-message").textContent.includes("No direct database fallback"));
        assert.equal(writes.at(-1).action, "unban");

        await page.setViewportSize({ width: 390, height: 844 });
        assert.equal(await page.evaluate(() => document.documentElement.scrollWidth <= window.innerWidth), true, "no mobile page overflow");
        if (process.env.HUB_TEST_SCREENSHOT) await page.screenshot({ path: process.env.HUB_TEST_SCREENSHOT, fullPage: true });
        allowed = false;
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator("#accounts-tab").isVisible(), false);
        assert.equal(await page.locator("#accounts-view").isVisible(), false);
        allowed = true; enabled = false;
        await page.evaluate(() => loadStatus());
        await page.locator("#accounts-tab").click();
        assert.equal(await page.locator("#accounts-disabled").isVisible(), true);
        assert.equal(await page.locator("#accounts-workspace").isVisible(), false);
        assert.deepEqual(errors, []);
        console.log("Hub account browser checks passed: create/edit, bans, mutes, GM, RBAC, IP bans, routing errors, permissions and mobile layout.");
    } finally { await browser.close(); }
})().catch((error) => { console.error(error); process.exitCode = 1; });
