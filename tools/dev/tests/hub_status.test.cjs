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
        let restart = {}, restartRequests = 0, mapRestartRequests = 0;
        let rejectRestart = false;
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
                    accountsEnabled: true, uptimeSeconds: 60 + requestCount, ...restart,
                    components: [{ ...component, uptimeSeconds: 60 + requestCount, players: requestCount }, ...registryNodes] };
                try {
                    if (slow) await new Promise(resolve => { releaseStatus = resolve; });
                    if (fail) return await json({ error: "temporarily unavailable" }, 503);
                    return await json(data, authenticated ? 200 : 401);
                } finally { --inFlight; }
            }
            if (url.pathname === '/api/v1/services/all/restart') {
                assert.equal(route.request().headers()['x-hub-csrf'],'token');
                if (rejectRestart) return json({error:'Mapserver needs an external refresh.'},409);
                ++restartRequests; restart={restartActive:true,restartState:'countdown',restartMessage:'World shutdown countdown; hub stays online.'};
                return json({accepted:true},202);
            }
            if (url.pathname === '/api/v1/cluster/maps-east/restart') {
                assert.equal(route.request().headers()['x-hub-csrf'],'token'); ++mapRestartRequests;
                return json({accepted:true},202);
            }
            if (url.pathname === "/api/v1/services/world/stop") {
                await new Promise(resolve => { releaseOperation = resolve; });
                return json({ accepted: true }, 202);
            }
            if (url.pathname === "/api/v1/backup/schedules") return json({canEdit: true, schedules: [
                {target:"hub", enabled:0, mode:"interval", intervalMinutes:60, minuteOfDay:0, weekday:0, revision:1}
            ]});
            if (url.pathname.startsWith("/api/v1/accounts/")) return json({ items: [], next: 0 });
            if (url.pathname === "/api/v1/logout") { authenticated = false; return json({ ok: true }); }
            const name = url.pathname === "/" ? "index.html" : url.pathname.slice(1);
            if (["index.html", "app.js", "status.js", "accounts.js", "backup.js", "metrics.js", "navigation.js", "app.css"].includes(name))
                return route.fulfill({ contentType: name.endsWith("js") ? "text/javascript" : name.endsWith("css") ? "text/css" : "text/html", body: fs.readFileSync(path.join(web, name)) });
            return route.fulfill({ status: 404, body: "" });
        });
        await page.goto("http://hub.test/");
        await page.locator("#health-view").waitFor({ state: "visible" });
        const resize = page.locator('#sidebar-resize');
        const sidebar = page.locator('#server-sidebar');
        const toggle = page.locator('#sidebar-toggle');
        const grip = await resize.boundingBox();
        await page.mouse.move(grip.x + grip.width/2, grip.y + 12);
        await page.mouse.down();
        await page.mouse.move(grip.x + grip.width/2 + 120, grip.y + 12);
        await page.mouse.up();
        assert.equal(Number(await resize.getAttribute('aria-valuenow')),412);
        await resize.focus(); await page.keyboard.press('ArrowLeft');
        assert.equal(Number(await resize.getAttribute('aria-valuenow')),392);
        await toggle.click();
        assert.equal(await sidebar.isVisible(),false);
        assert.equal(await toggle.getAttribute('aria-expanded'),'false');
        const hiddenCount=count;
        await page.waitForFunction(() => document.querySelector('#last-updated').textContent.startsWith('Live'));
        await page.evaluate(() => loadStatus());
        assert.ok(count>hiddenCount,'folding keeps status polling active');
        await page.reload();
        await page.locator('#health-view').waitFor({state:'visible'});
        assert.equal(await sidebar.isVisible(),false,'folded state survives reload');
        await toggle.click();
        assert.equal(Number(await resize.getAttribute('aria-valuenow')),392,'width survives reload');
        await resize.focus(); await page.keyboard.press('Home');
        registryNodes=Array.from({length:12},(_,i)=>({...component,key:`world-${i}`,name:`World node ${i} with a very long descriptive name`}));
        await page.evaluate(() => loadStatus());
        assert.equal(await sidebar.evaluate(el=>el.scrollHeight>el.clientHeight),true);
        const bottom=page.locator('#component-grid article').last().getByRole('button',{name:'Stop',exact:true});
        await bottom.scrollIntoViewIfNeeded();
        const buttonBox=await bottom.boundingBox(), sidebarBox=await sidebar.boundingBox();
        assert.ok(buttonBox.x>=sidebarBox.x && buttonBox.x+buttonBox.width<=sidebarBox.x+sidebarBox.width,'controls fit horizontally');
        assert.ok(buttonBox.y>=sidebarBox.y && buttonBox.y+buttonBox.height<=sidebarBox.y+sidebarBox.height,'last controls are reachable by scrolling');
        if (process.env.HUB_TEST_SIDEBAR_SCREENSHOT) await page.screenshot({path:process.env.HUB_TEST_SIDEBAR_SCREENSHOT});
        await page.setViewportSize({width:390,height:600});
        assert.equal(await resize.isVisible(),false);
        assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),true,'sidebar fits mobile');
        await toggle.click(); await toggle.click();
        assert.equal(await sidebar.isVisible(),true);
        await page.setViewportSize({width:1280,height:800});
        registryNodes=[]; await page.evaluate(() => loadStatus());
        await sidebar.evaluate(el=>{el.scrollTop=0;});
        await page.evaluate(()=>window.scrollTo(0,0));
        assert.equal(await page.locator("#status-view").isVisible(), false);
        assert.equal(await page.locator("#backup-view").isVisible(), false);
        assert.equal(await page.locator("#health-dashboard svg:visible").count(), 3);
        await page.evaluate(() => {
            const now=Date.now(), data=healthDashboard.latest;
            for (let i=0;i<130;i++) healthDashboard.update(data,now+i*5000);
            if (healthDashboard.history.get('world').length !== 120) throw Error('History must be bounded');
            healthDashboard.stale();
            if (healthDashboard.history.get('world').at(-1).cpu !== null) throw Error('Connection loss must break graph lines');
            healthDashboard.update({...data, services:data.services.map(item=>({...item,cpu:null}))},now+650000);
        });
        assert.equal(await page.locator('.metric-value').nth(1).textContent(), 'Metrics unavailable');
        await page.evaluate(() => { healthDashboard.reset(); return loadStatus(); });
        if (process.env.HUB_TEST_HEALTH_SCREENSHOT) await page.screenshot({path:process.env.HUB_TEST_HEALTH_SCREENSHOT,fullPage:true});
        await page.setViewportSize({width:390,height:844});
        assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth <= innerWidth),true,'health fits mobile');
        await page.setViewportSize({width:1280,height:800});
        await page.locator('#backup-tab').click();
        await page.locator('.backup-schedule input[type=number]').fill('125');
        await page.evaluate(() => loadStatus());
        assert.equal(await page.locator('#backup-view').isVisible(),true);
        assert.equal(await page.locator('.backup-schedule input[type=number]').inputValue(),'125');
        await page.locator("#status-tab").click();
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
        await page.locator('#restart-all-nodes').click();
        await page.waitForFunction(()=>document.querySelector('#node-restart-status').textContent.includes('countdown'));
        assert.equal(restartRequests,1);
        assert.equal(await page.locator('#restart-all-nodes').isDisabled(),true);
        assert.equal(await page.locator('#component-grid .stop').isDisabled(),true);
        restart={restartActive:false,restartState:'completed',restartMessage:'Nodes ready; hub stayed online.'};
        await page.evaluate(()=>loadStatus());
        assert.equal(await page.locator('#restart-all-nodes').isEnabled(),true);
        rejectRestart = true;
        await page.locator('#restart-all-nodes').click();
        await page.waitForFunction(()=>document.querySelector('#node-restart-status').textContent.includes('external refresh'));
        await page.evaluate(() => loadStatus());
        assert.match(await page.locator('#node-restart-status').textContent(),/external refresh/,'polling retains rejection details');
        rejectRestart = false;
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
        registryNodes = [{key:'cluster:maps-east',name:'Eastern map data',mapserver:true,clusterKey:'maps-east',clusterCanAdmin:true,
            live:true,canRestart:true,adminState:'enabled',managed:false,status:'online',state:'running',metricsAvailable:true,
            uptimeSeconds:60,cpuPercent:2,memoryMiB:64,transfers:1,sentKiB:1024,failures:0,requests:5,assets:40,maps:[0]}];
        await page.evaluate(() => loadStatus());
        const mapCard = page.locator('#component-grid article').filter({hasText:'Eastern map data'});
        assert.equal(await mapCard.locator('.service-metrics').isVisible(),true);
        assert.match(await mapCard.locator('.service-metrics').textContent(),/Memory 64 MiB.*Transfers 1/);
        assert.equal(await mapCard.getByRole('button',{name:'Start',exact:true}).isVisible(),false);
        await mapCard.getByRole('button',{name:'Restart',exact:true}).click();
        await page.waitForFunction(()=>document.querySelector('#service-action-message').textContent.includes('Restart requested'));
        assert.equal(mapRestartRequests,1);
        registryNodes[0].metricsAvailable=false; registryNodes[0].live=false; registryNodes[0].status='offline'; registryNodes[0].state='offline';
        await page.evaluate(() => loadStatus());
        assert.match(await mapCard.locator('.service-metrics').textContent(),/unavailable/);
        registryNodes = [{ key: "ingress:Authnet ingress", name: "Authnet ingress", status: "online",
            detail: "127.0.0.1:1118 | active 1 | routed 3 | rejected 0", managed: false }];
        await page.evaluate(() => loadStatus());
        const ingress = page.locator("#component-grid article").nth(1);
        assert.equal(await ingress.getByRole("button", { name: "Start", exact: true }).isVisible(), false);
        await ingress.evaluate(el => { window.originalIngress = el; });
        registryNodes[0].detail = "127.0.0.1:1118 | active 2 | routed 4 | rejected 1";
        await page.evaluate(() => loadStatus());
        assert.ok((await ingress.textContent()).includes("routed 4"));
        assert.equal(await ingress.evaluate(el => window.originalIngress === el), true, "routing updates must retain the card DOM");
        registryNodes = [];
        await page.evaluate(() => loadStatus());
        assert.equal(await page.evaluate(() => window.originalStop === document.querySelector("#component-grid .stop")), true);
        slow = true;
        const refresh = page.evaluate(() => Promise.all([loadStatus(), loadStatus(), loadStatus()]));
        while (!releaseStatus) await new Promise(resolve => setTimeout(resolve, 20));
        await new Promise(resolve => setTimeout(resolve, 1200));
        assert.equal(maxInFlight, 1, "only one status request may run at once");
        slow = false; releaseStatus(); releaseStatus = null;
        await refresh;
        registryNodes = [{key:'hub',name:'Hub Runtime',status:'online',detail:'Control process is running'}];
        await page.evaluate(() => loadStatus());
        const hubCard=page.locator('#component-grid article').filter({hasText:'Hub Runtime'});
        assert.equal(await hubCard.locator('.status-label').textContent(),'online');
        fail = true;
        await page.evaluate(() => loadStatus());
        assert.match(await page.locator("#last-updated").textContent(), /Connection lost/);
        assert.equal(await hubCard.locator('.status-label').textContent(),'offline');
        assert.match(await hubCard.getAttribute('class'),/offline/);
        assert.match(await page.locator('#status-summary').textContent(),/Hub offline/);
        assert.equal(await page.locator('#component-grid .stop').first().isDisabled(),true);
        assert.equal(await page.locator("#world-console").isVisible(), true);
        fail = false;
        await page.waitForFunction(() => document.querySelector("#last-updated").textContent.startsWith("Live"));
        assert.equal(await hubCard.locator('.status-label').textContent(),'online');
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
