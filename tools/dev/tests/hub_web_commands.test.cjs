"use strict";

// Exercise the shipped dashboard script with an isolated DOM and HTTP stub.
// Never connects to a running hub or sends real server commands.
const assert = require("node:assert/strict");
const fs = require("node:fs");
const path = require("node:path");
const vm = require("node:vm");

class Element {
    constructor() { this.listeners = {}; this.value = ""; this.textContent = ""; }
    addEventListener(name, callback) { this.listeners[name] = callback; }
    replaceChildren(...children) { this.children = children; }
    append(...children) { this.children = [...(this.children || []), ...children]; }
}

const elements = new Map();
const element = (id) => {
    if (!elements.has(id)) elements.set(id, new Element());
    return elements.get(id);
};
const requests = [];
let reply = { ok: true, json: async () => ({ accepted: true }) };
const context = vm.createContext({
    document: { querySelector: element, createElement: () => new Element(), addEventListener() {} },
    window: { setTimeout() {}, setInterval() { return 1; }, clearInterval() {} },
    TextEncoder, URLSearchParams, Date, console,
    fetch: async (url, options) => { requests.push({ url, options }); return reply; }
});
let source = fs.readFileSync(path.resolve(__dirname, "../../../src/server/hub/web/app.js"), "utf8");
// Suppress only the automatic initial refresh; tests drive status explicitly.
assert.match(source, /loadStatus\(\);\s*$/);
source = source.replace(/loadStatus\(\);\s*$/, "");
vm.runInContext(source, context);

function status(overrides = {}, admin = true) {
    context.statusData = {
        csrfToken: "test-csrf", username: "tester", uptimeSeconds: 90,
        canOperateServices: true, canSendWorldCommands: admin,
        components: [{ key: "world", name: "World", status: "online", state: "running",
            enabled: true, managed: true, canSendCommands: true, commandPending: false,
            ...overrides }]
    };
    vm.runInContext("renderStatus(statusData)", context);
}
const submit = () => element("#world-command-form").listeners.submit({ preventDefault() {} });

(async () => {
    status();
    assert.equal(element("#world-command-send").disabled, false);
    element("#world-command").value = ".server restart 300";
    await submit();
    assert.equal(requests.length, 1);
    assert.equal(requests[0].url, "/api/v1/services/world/command");
    assert.equal(requests[0].options.headers["X-Hub-CSRF"], "test-csrf");
    assert.equal(requests[0].options.body.get("command"), ".server restart 300");
    assert.match(element("#world-command-result").textContent, /queued/);
    await submit();
    assert.equal(requests.length, 1, "prevent repeat submission until status refresh");

    status({ commandPending: true });
    await submit();
    assert.equal(requests.length, 1, "pending command blocks another request");
    status({ state: "stopped" });
    await submit();
    assert.equal(requests.length, 1, "offline world blocks submission");
    status({ canSendCommands: false });
    assert.equal(element("#world-command-send").disabled, true, "old binary blocks submission");
    status({}, false);
    assert.equal(element("#world-console").hidden, true);
    await submit();
    assert.equal(requests.length, 1, "operator cannot send console commands");

    status();
    for (const invalid of ["server info\nserver shutdown 0", "é".repeat(513), ""]) {
        element("#world-command").value = invalid;
        await submit();
        assert.equal(requests.length, 1, "invalid command must not reach HTTP");
    }

    element("#world-command").value = "server shutdown time 23:00";
    status();
    assert.equal(element("#world-command").value, "server shutdown time 23:00", "refresh preserves typing");
    reply = { ok: false, json: async () => ({ error: "worldserver is not ready" }) };
    await submit();
    assert.equal(element("#world-command-result").textContent, "worldserver is not ready");
    status({ commandResult: "ERROR Unknown command <script>bad</script>" });
    assert.equal(element("#world-command-result").textContent, "ERROR Unknown command <script>bad</script>");
    vm.runInContext("showLogin()", context);
    assert.equal(element("#world-command").value, "");
    assert.equal(element("#world-console").hidden, true);
    assert.equal(element("#world-command-result").textContent, "No command response yet.");
    console.log("Hub web command regression checks passed.");
})().catch((error) => { console.error(error); process.exitCode = 1; });
