/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
const byId = id => document.getElementById(id);
let csrf = "", snapshot = null, socket = null, retry = null, cursor = null, backoff = 1000, signedIn = false, sending = false, generation = 0;
const pages = new window.HubPageNavigation();
const healthDashboard = new window.HubHealthDashboard(byId('control-health-dashboard'));
let targetSignature = "", connected = false;
const operations = new Map(), serviceCards = new Map();
const backupSchedules = new window.HubBackupSchedules(byId('backup-schedules'), (value, section = 'schedules') => api('backup/' + section,value));
function text(element, value) { if (element.textContent !== String(value)) element.textContent = String(value); }
async function api(path, body) {
    const response = await fetch(`/control/v1/${path}`, {
        method: body ? "POST" : "GET", credentials: "same-origin", cache: "no-store",
        headers: body ? { "Content-Type": "application/json", "X-Control-CSRF": csrf } : {},
        body: body ? JSON.stringify(body) : undefined, signal: AbortSignal.timeout(8000)
    });
    const value = await response.json();
    if (value.version !== 1) throw new Error("Incompatible control protocol");
    if (!response.ok) {
        if (response.status === 401) reset("Your session expired. Sign in again.");
        throw new Error(value.payload?.message || "Request failed");
    }
    return value.payload;
}
function connection(message, stale = false) { text(byId("connection"), message); byId("connection").classList.toggle("stale", stale); }
function reset(message = "") {
    backupSchedules.reset(); healthDashboard.reset(); pages.reset(); byId("control-nav").hidden = true;
    ++generation; connected = false; signedIn = false; csrf = ""; snapshot = cursor = null;
    clearTimeout(retry); retry = null;
    if (socket) { const old = socket; socket = null; old.close(); }
    byId("workspace").hidden = byId("logout").hidden = true;
    byId("login-panel").hidden = false; text(byId("login-error"), message);
    byId("password").value = ""; operations.clear(); serviceCards.clear(); byId("services").replaceChildren();
    byId("events").replaceChildren(); byId("no-events").hidden = false;
    targetSignature = ""; byId("command-form").reset(); text(byId("command-result"), "");
}
function uptime(seconds) {
    const value = Math.max(0, Number(seconds || 0));
    return `${Math.floor(value / 3600)}h ${Math.floor(value % 3600 / 60)}m ${value % 60}s`;
}
function table(id, headings, rows, empty) {
    const holder = byId(id), signature = JSON.stringify(rows);
    if (holder.dataset.signature === signature) return;
    holder.dataset.signature = signature;
    if (!rows.length) { const p = document.createElement("p"); p.className = "muted"; p.textContent = empty; holder.replaceChildren(p); return; }
    const table = document.createElement("table"), head = document.createElement("thead"), body = document.createElement("tbody"), tr = document.createElement("tr");
    for (const label of headings) { const th = document.createElement("th"); th.scope = "col"; th.textContent = label; tr.append(th); } head.append(tr);
    for (const row of rows) { const tr = document.createElement("tr"); for (const value of row) { const td = document.createElement("td"); td.textContent = value; tr.append(td); } body.append(tr); }
    table.append(head, body); holder.replaceChildren(table);
}
function targets() {
    if (!snapshot) return;
    const action = byId("action").value, cluster = action.startsWith("cluster."), world = action.startsWith("world.");
    const options = (cluster ? snapshot.nodes : snapshot.services).filter(item => cluster || item.enabled && (!world || item.world));
    const signature = JSON.stringify(options.map(item => [item.key, item.name]));
    if (targetSignature !== signature) {
        const selected = byId("target").value;
        byId("target").replaceChildren(...options.map(item => { const option = document.createElement("option"); option.value = item.key; option.textContent = `${item.name} (${item.key})`; return option; }));
        if (options.some(item => item.key === selected)) byId("target").value = selected;
        targetSignature = signature;
    }
    const scheduled = ["world.shutdown", "world.restart"].includes(action);
    byId("countdown-field").hidden = !scheduled;
    byId("send").disabled = sending || !connected || !options.length || snapshot.stale || !snapshot.permissions.operate;
    text(byId("command-help"), cluster ? "Routing changes affect new connections. Existing sessions remain connected." :
        world ? "World lifecycle commands use the normal player-save sequence. Zero seconds starts shutdown immediately." :
        "Only services already configured in the hub can be started or stopped. World stops use graceful shutdown.");
}
function render() {
    if (!snapshot) return;
    byId("workspace").hidden = byId("logout").hidden = false; byId("login-panel").hidden = true;
    byId('control-nav').hidden = false;
    byId('control-console-tab').hidden = !snapshot.permissions.operate;
    pages.refresh();
    healthDashboard.update({hubUptime:snapshot.hub?.uptimeSeconds||0,stale:snapshot.stale,services:snapshot.services.map(item=>({
        ...item,cpu:typeof item.cpuBasisPoints === "number" && item.cpuBasisPoints>=0?item.cpuBasisPoints/100:null,update:item.updateTimeMs}))});
    backupSchedules.update(snapshot.permissions.role === 'administrator');
    text(byId("hub-name"), `Hub · ${uptime(snapshot.hub?.uptimeSeconds)}`);
    text(byId("identity"), `${byId("identity").dataset.username || ""} · ${snapshot.permissions.role}`);
    byId("operations").hidden = !snapshot.permissions.operate;
    for (const service of snapshot.services) {
        let card = serviceCards.get(service.key);
        if (!card) { card = document.createElement("div"); card.className = "service"; card.append(document.createElement("strong"), document.createElement("p"), document.createElement("p")); card.children[1].className = "state"; serviceCards.set(service.key, card); byId("services").append(card); }
        text(card.children[0], service.name); text(card.children[1], service.state);
        text(card.children[2], `Uptime ${uptime(service.uptimeSeconds)}` + (service.world ? service.metricsAvailable ? `\n${service.players} players · CPU ${service.cpuBasisPoints < 0 ? "—" : (service.cpuBasisPoints / 100).toFixed(1) + "%"} · ${service.updateTimeMs} ms` : "\nPlayers / load unavailable" : ""));
    }
    for (const [key, card] of serviceCards) if (!snapshot.services.some(item => item.key === key)) { card.remove(); serviceCards.delete(key); }
    table("realms", ["Realm", "State", "Endpoint"], snapshot.realms.map(item => [item.name, `${item.state}${item.locked ? " · restricted" : ""}`, item.address ? `${item.address}:${item.port}` : "—"]), "No configured realms.");
    table("nodes", ["Node", "Service", "Readiness", "Policy", "Load", "Hub connections", "Lease"], snapshot.nodes.map(item => [item.name, item.service, !item.live ? "Offline" : item.ready ? "Ready" : "Not ready", item.policy, `${item.load} / ${item.capacity || "—"}`, `${item.leaseSeconds}s`]), "No registered nodes.");
    table("routes", ["Ingress", "Active", "Routed", "Rejected", "Failures", "Eligible"], snapshot.routes.map(item => [item.name, item.active, item.routed, item.rejected, item.failures, item.eligibleNodes]), "No routing listeners.");
    targets(); text(byId("updated"), `Updated ${new Date().toLocaleTimeString()}`);
    if (snapshot.stale) connection("Hub unavailable · values may be stale", true);
}
function result(payload) {
    operations.set(payload.requestId, payload.state);
    while (operations.size > 20) operations.delete(operations.keys().next().value);
    byId("events").replaceChildren(...Array.from(operations, ([id, state]) => { const li = document.createElement("li"); li.textContent = `${state} · ${id}`; return li; }).reverse());
    byId("no-events").hidden = !!operations.size;
    text(byId("command-result"), `${payload.state} · ${payload.requestId}\n${payload.state === "dispatched" ? "The hub dispatched the request. Watch server status for the resulting transition." : payload.state === "unknown" ? "Outcome is uncertain. Inspect server state before submitting another command." : "Watch this result and server status for progress."}`);
}
function connect() {
    if (!signedIn || socket) return;
    const current = generation;
    const ws = new WebSocket(`${location.protocol === "https:" ? "wss:" : "ws:"}//${location.host}/control/v1/events`); socket = ws;
    ws.onopen = () => { if (current !== generation) { ws.close(); return; } ws.send(JSON.stringify({ version: 1, type: "subscribe", csrf, ...(cursor ? { cursor } : {}) })); };
    ws.onmessage = event => {
        if (current !== generation) return;
        let value; try { value = JSON.parse(event.data); } catch { ws.close(); return; }
        if (value.version !== 1) { reset("Incompatible control protocol."); return; }
        if (value.type === "resync_required") { cursor = null; connection("Resynchronizing status…", true); return; }
        if (value.type === "snapshot") { snapshot = value.payload; for (const job of snapshot.commands || []) result(job); }
        else if (["delta", "command.result", "checkpoint"].includes(value.type)) {
            if (!cursor || value.epoch !== cursor.epoch || value.sequence !== cursor.sequence + 1) { cursor = null; ws.close(); return; }
            if (value.type === "delta") snapshot = { ...snapshot, ...value.payload };
            if (value.type === "command.result") result(value.payload);
        }
        if (value.type !== "heartbeat") cursor = { epoch: value.epoch, sequence: value.sequence };
        connected = true; backoff = 1000; render(); if (!snapshot?.stale) connection("Live · connected");
    };
    ws.onclose = event => {
        if (current !== generation || socket !== ws) return;
        socket = null;
        if (event.code === 1008) { reset("Session ended or access changed. Sign in again."); return; }
        connection("Disconnected · reconnecting (values may be stale)", true);
        connected = false; healthDashboard.stale(); if (snapshot) targets();
        retry = setTimeout(connect, backoff); backoff = Math.min(backoff * 2, 15000);
    };
}
async function enter(payload) {
    csrf = payload.csrf; byId("identity").dataset.username = payload.username;
    const value = await api("status"); snapshot = value.status; csrf = value.csrf;
    signedIn = true; cursor = null; render(); connect();
}
byId("login-form").addEventListener("submit", async event => {
    event.preventDefault(); const button = event.target.querySelector("button"); button.disabled = true;
    try { await enter(await api("session", { username: byId("username").value, password: byId("password").value })); byId("password").value = ""; text(byId("login-error"), ""); }
    catch (error) { text(byId("login-error"), error.message); } finally { button.disabled = false; }
});
byId("logout").addEventListener("click", async () => { try { await api("logout", {}); } finally { reset(); } });
byId("action").addEventListener("change", targets);
byId("command-form").addEventListener("submit", async event => {
    event.preventDefault(); if (sending) return;
    const current = generation, action = byId("action").value, payload = { action, target: byId("target").value };
    if (["world.restart", "world.shutdown"].includes(action)) payload.seconds = Number(byId("seconds").value);
    const requestId = crypto.randomUUID().replaceAll("-", "");
    sending = true; targets(); text(byId("command-result"), `Submitting ${requestId}…`);
    try { const response = await api("commands", { version: 1, type: "command", requestId, timestamp: new Date().toISOString(), payload }); if (current === generation) result(response); }
    catch (error) { if (current === generation) text(byId("command-result"), `${error.message}\nRequest ${requestId}. Commands are never retried automatically.`); }
    finally { sending = false; if (current === generation) targets(); }
});
(async () => { try { const value = await api("status"); await enter(value); } catch { reset(); } })();
