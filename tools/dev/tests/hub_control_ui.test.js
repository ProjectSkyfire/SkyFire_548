/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
// Exercise independent rendering and reconnect state without a browser or live server.
const assert = require("node:assert/strict"), fs = require("node:fs"), vm = require("node:vm"), path = require("node:path");
class Element {
    constructor() { this.children = []; this.dataset = {}; this.value = ""; this.textContent = ""; this.hidden = false; this.classList = { toggle() {} }; }
    append(...items) { this.children.push(...items); }
    replaceChildren(...items) { this.children = items; }
    addEventListener() {}
    setAttribute() {}
    remove() {}
    reset() {}
}
const elements = new Map();
const document = { getElementById(id) { if (!elements.has(id)) elements.set(id, new Element()); return elements.get(id); }, createElement() { return new Element(); } };
const sockets = [];
class WebSocket {
    constructor() { sockets.push(this); }
    send(value) { this.sent = JSON.parse(value); }
    close() { this.closed = true; }
}
const context = vm.createContext({ window: {}, document, WebSocket, location: { protocol: "https:", host: "localhost" },
    fetch: () => new Promise(() => {}), AbortSignal, setTimeout: () => 1, clearTimeout() {}, console });
vm.runInContext(fs.readFileSync(path.join(__dirname, "../../../src/server/hub/web/backup.js"), "utf8"), context);
vm.runInContext(fs.readFileSync(path.join(__dirname, "../../../src/server/hub/control/gui/control.js"), "utf8"), context);
const state = { hub: { uptimeSeconds: 42 }, permissions: { role: "operator", operate: true }, stale: false,
    services: [{ key: "world", name: "World one", enabled: true, world: true, state: "running", uptimeSeconds: 40, metricsAvailable: true, players: 5, cpuBasisPoints: 1200, updateTimeMs: 7 }],
    nodes: [{ key: "world", name: "World one", service: "world", ready: true, live: true, policy: "enabled", load: 5, capacity: 100, leaseSeconds: 15 }], realms: [], routes: [] };
document.getElementById("action").value = "world.restart";
vm.runInContext(`snapshot = ${JSON.stringify(state)}; connected = true; signedIn = true; csrf = "test"; render();`, context);
document.getElementById("target").value = "world";
document.getElementById("seconds").value = "725";
const targetOption = document.getElementById("target").children[0];
const serviceCard = document.getElementById("services").children[0];
for (let i = 0; i < 100; ++i) vm.runInContext("snapshot.hub.uptimeSeconds++; snapshot.services[0].players++; render();", context);
assert.equal(document.getElementById("seconds").value, "725");
assert.equal(document.getElementById("target").value, "world");
assert.equal(document.getElementById("target").children[0], targetOption);
assert.equal(document.getElementById("services").children[0], serviceCard);
assert.match(serviceCard.children[2].textContent, /105 players/);
vm.runInContext("connect();", context);
let ws = sockets.at(-1); ws.onopen();
ws.onmessage({ data: JSON.stringify({ version: 1, type: "snapshot", epoch: "e", sequence: 1, payload: state }) });
assert.equal(document.getElementById("send").disabled, false);
ws.onclose({ code: 1006 }); assert.equal(document.getElementById("send").disabled, true);
vm.runInContext("connect();", context); ws = sockets.at(-1); ws.onopen();
assert.equal(ws.sent.cursor.sequence, 1);
ws.onmessage({ data: JSON.stringify({ version: 1, type: "delta", epoch: "e", sequence: 2, payload: { hub: { uptimeSeconds: 99 } } }) });
assert.equal(document.getElementById("send").disabled, false);
assert.equal(document.getElementById("seconds").value, "725");
ws.onmessage({ data: JSON.stringify({ version: 1, type: "delta", epoch: "e", sequence: 4, payload: {} }) });
assert.equal(ws.closed, true);
assert.equal(vm.runInContext("cursor", context), null);
console.log("PASS 100 independent status refreshes preserve form values/DOM; reconnect restores actions and gaps require resync.");
