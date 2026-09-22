"use strict";

// Independently refreshed sidebar. Consumers never need to poll or rebuild pages.
window.HubStatus = (() => {
    const grid = document.querySelector("#component-grid");
    const summary = document.querySelector("#status-summary");
    const user = document.querySelector("#signed-in-user");
    const updated = document.querySelector("#last-updated");
    const actionMessage = document.querySelector("#service-action-message");
    const restartAll = document.querySelector("#restart-all-nodes");
    const restartStatus = document.querySelector("#node-restart-status");
    const fallbackStatus = document.querySelector("#world-fallback-status");
    let restartSending = false;
    let restartError = '';
    const online = document.createElement("strong");
    const uptime = document.createElement("span");
    summary.replaceChildren(online, uptime);
    const cards = new Map();
    const pending = new Set();
    let callbacks = {}, snapshot = null, timer = null, request = null, controller = null;
    let running = false, generation = 0, connectionLost = false;
    const text = (element, value) => { if (element.textContent !== value) element.textContent = value; };
    const isWorld = component => component.isWorld === true || component.key === "world";
    function formatUptime(seconds) {
        seconds = Math.max(0, Math.floor(seconds || 0));
        const days = Math.floor(seconds / 86400), hours = Math.floor(seconds % 86400 / 3600);
        const minutes = Math.floor(seconds % 3600 / 60), parts = [];
        if (days) parts.push(`${days}d`);
        if (hours || days) parts.push(`${hours}h`);
        if (minutes || hours || days) parts.push(`${minutes}m`);
        parts.push(`${seconds % 60}s`);
        return parts.join(" ");
    }
    function createCard(key) {
        const card = { key };
        card.article = document.createElement("article");
        const heading = document.createElement("div");
        heading.className = "component-heading";
        card.title = document.createElement("h3");
        card.state = document.createElement("span");
        card.state.className = "status-label";
        heading.append(card.title, card.state);
        card.detail = document.createElement("p");
        card.metrics = document.createElement("p");
        card.metrics.className = "service-metrics";
        card.controls = document.createElement("div");
        card.controls.className = "service-controls";
        for (const action of ["start", "stop", "restart", "promote", "edit", "drain", "disable", "enable"]) {
            const button = document.createElement("button");
            button.type = "button";
            button.className = `service-button ${action}`;
            button.textContent = action[0].toUpperCase() + action.slice(1);
            button.addEventListener("click", () => action === "edit" ? callbacks.onEdit?.(card.data) : operate(key, action));
            card[action] = button;
            card.controls.append(button);
        }
        card.article.append(heading, card.detail, card.metrics, card.controls);
        return card;
    }
    function updateCard(card, component, data) {
        card.data = component;
        const active = ["starting", "running", "standby", "unresponsive", "stopping"].includes(component.state);
        card.article.className = `component ${component.status}`;
        text(card.title, component.name);
        text(card.state, component.clusterKey && component.live && component.adminState !== "enabled" ? component.adminState : component.status);
        text(card.detail, component.detail || "");
        card.metrics.hidden = !component.managed && !component.mapserver && !component.characterserver && !component.chatserver;
        const cluster = component.clusterCanAdmin === true;
        card.controls.hidden = !component.managed && !cluster && !component.canRestart;
        card.start.hidden = card.stop.hidden = !component.managed;
        const pairMember = data.fallback?.enabled && [data.fallback.primary, data.fallback.standby].includes(component.key);
        card.promote.hidden = !component.managed || !pairMember;
        card.promote.disabled = (active && component.state !== 'standby') || pending.has(component.key) || !data.canOperateServices || data.restartActive;
        card.promote.title = 'Gracefully stop the active world, confirm exclusive ownership, then activate this world. Players must reconnect.';
        card.restart.hidden = !component.canRestart;
        card.restart.disabled = pending.has(component.key) || !(component.managed ? active && component.state !== 'stopping' : component.live) || !data.canOperateServices || data.restartActive;
        card.restart.title = component.chatserver ? 'Restart the chat daemon independently.' : 'Data-service restart requires graceful world shutdown first.';
        let metrics = `Uptime ${active ? formatUptime(component.uptimeSeconds) : "—"}`;
        if (isWorld(component)) {
            metrics += component.state === 'standby' ? '\nStatic data loaded · Waiting for promotion' : component.state === "running" && component.metricsAvailable
                ? ` · ${component.players} players\nCPU ${component.cpuPercent == null ? "—" : component.cpuPercent.toFixed(1) + "%"} · Update ${component.updateTimeMs} ms`
                : "\nPlayers / load unavailable";
            card.metrics.title = "CPU: total logical CPU capacity. Update: world tick processing time, excluding sleep. Metrics expire after 15 seconds without a new tick.";
        }
        if (component.mapserver) {
            metrics = component.metricsAvailable
                ? `Uptime ${formatUptime(component.uptimeSeconds)} · Maps ${(component.maps||[]).join(', ')}\nCPU ${component.cpuPercent.toFixed(1)}% · Memory ${component.memoryMiB} MiB · Transfers ${component.transfers}\nSent ${(component.sentKiB/1024).toFixed(1)} MiB · Errors ${component.failures}`
                : 'Mapserver metrics unavailable';
            card.metrics.title = 'Metrics reported through the hub every 5 seconds; expire after 15 seconds. CPU is a percentage of total logical CPU capacity.';
        }
        if (component.characterserver) {
            metrics = component.metricsAvailable
                ? `Uptime ${formatUptime(component.uptimeSeconds)} · DB ${component.databaseReady ? 'ready' : 'unavailable'}\nCPU ${component.cpuPercent.toFixed(1)}% · Memory ${component.memoryMiB} MiB\nConnections ${component.connections} · Pending ${component.pendingRequests}\nReads ${component.reads} · Writes ${component.writes} · Batches ${component.transactions}\nAverage ${component.latencyMs.toFixed(2)} ms · Errors ${component.failures}\nLast commit ${component.lastCommitAgeSeconds == null ? '—' : component.lastCommitAgeSeconds + 's ago'}`
                : 'Character-server metrics unavailable';
            card.metrics.title = 'Metrics expire after 15 seconds. Pending includes queued and executing requests; latency includes queue time. Write totals count acknowledged requests, not saved characters.';
        }
        if (component.chatserver) {
            metrics = component.metricsAvailable
                ? `Uptime ${formatUptime(component.uptimeSeconds)} � Realms ${(component.chatRealms || []).join(', ')}\nPresent ${component.presencePlayers ?? 0} � Connections ${component.connections} � Requests ${component.requests} � Errors ${component.failures}\nWhisper relays ${component.whisperRelays ?? 0} � Other routing pending`
                : 'Chat-server metrics unavailable';
            card.metrics.title = 'Requests include health, presence and whisper relays. Relay counts do not confirm delivery to a player. Metrics expire after 15 seconds.';
        }
        text(card.metrics, metrics);
        card.start.disabled = pending.has(component.key) || !data.canOperateServices || !component.enabled || active;
        card.stop.disabled = pending.has(component.key) || !data.canOperateServices || !active || component.state === "stopping";
        card.edit.hidden = !isWorld(component) || data.canSendWorldCommands !== true;
        for (const action of ["drain", "disable", "enable"]) {
            card[action].hidden = !cluster;
            const state = { drain: "draining", disable: "disabled", enable: "enabled" }[action];
            card[action].disabled = pending.has(component.key) || !data.canOperateServices || component.adminState === state;
            card[action].title = action === "enable" ? "Allow new authentication connections or world handoffs when the node is ready." :
                "Stop new authentication connections or world handoffs. Existing sessions continue; the process remains running.";
        }
        if (data.restartActive) for (const action of ["start","stop","restart","promote","edit","drain","disable","enable"]) card[action].disabled = true;
        if (connectionLost) {
            card.article.className = 'component offline';
            text(card.state, component.key === 'hub' ? 'offline' : 'unknown');
            text(card.detail, component.key === 'hub' ? 'Hub is unreachable; waiting for a successful status response.' : 'Live status unavailable; last report is stale.');
            text(card.metrics, 'Live metrics unavailable');
            for (const action of ['start','stop','restart','promote','edit','drain','disable','enable']) card[action].disabled = true;
        } else card.edit.disabled = !!data.restartActive;
    }
    function markStale() {
        connectionLost = true;
        if (restartAll) restartAll.disabled = true;
        text(updated, 'Connection lost · retrying');
        text(online, 'Hub offline · status unavailable');
        text(uptime, 'Hub uptime unavailable');
        if (snapshot) for (const card of cards.values()) updateCard(card,card.data,snapshot);
        callbacks.onStale?.();
    }
    function render(data) {
        if (fallbackStatus) {
            fallbackStatus.hidden = !data.fallback?.enabled;
            text(fallbackStatus, data.fallback?.enabled ? `World fallback (${data.fallback.automatic ? 'automatic' : 'manual'}): ${data.fallback.message}` : '');
        }
        if (restartAll) {
            restartAll.disabled = restartSending || data.restartActive || !data.canOperateServices;
            restartAll.title = 'Gracefully restart running worlds, authentication, mapserver and managed chat nodes. Hub/web stays online.';
            if (data.restartActive) restartError = '';
            text(restartStatus, restartError || data.restartMessage || 'Restart refreshes running server nodes; the hub stays online.');
        }
        connectionLost = false;
        snapshot = data;
        text(user, data.username);
        text(updated, `Live · ${new Date().toLocaleTimeString()}`);
        text(online, `${data.components.filter(item => item.status === "online").length} / ${data.components.length} online`);
        text(uptime, `Hub uptime ${formatUptime(data.uptimeSeconds)}`);
        const keys = new Set(data.components.map(item => item.key));
        for (const [key, card] of cards) {
            if (!keys.has(key)) { card.article.remove(); cards.delete(key); }
        }
        data.components.forEach((component, index) => {
            let card = cards.get(component.key);
            if (!card) { card = createCard(component.key); cards.set(component.key, card); }
            updateCard(card, component, data);
            // Do not detach/reinsert existing controls on ordinary status changes.
            if (grid.children[index] !== card.article) grid.insertBefore(card.article, grid.children[index] || null);
        });
        callbacks.onStatus?.(data);
    }
    async function operate(key, action) {
        if (!running || connectionLost || !snapshot || pending.has(key)) return;
        const operationGeneration = generation;
        pending.add(key);
        updateCard(cards.get(key), cards.get(key).data, snapshot);
        const component = cards.get(key).data;
        const cluster = ["drain", "disable", "enable"].includes(action) || (action === 'restart' && !component.managed);
        const label = action[0].toUpperCase() + action.slice(1);
        const progress = { start: "Starting", stop: "Stopping", restart: "Restarting", promote: "Promoting", drain: "Draining", disable: "Disabling", enable: "Enabling" }[action];
        text(actionMessage, `${progress} ${component.name}...`);
        try {
            const path = cluster ? `/api/v1/cluster/${encodeURIComponent(component.clusterKey)}/${action}` :
                `/api/v1/services/${encodeURIComponent(key)}/${action}`;
            const response = await fetch(path, {
                method: "POST", credentials: "same-origin", headers: { "X-Hub-CSRF": snapshot.csrfToken }
            });
            const data = await response.json();
            if (operationGeneration !== generation) return;
            if (response.status === 401) { stop(); callbacks.onUnauthorized?.(); return; }
            if (!response.ok) throw new Error(data.error || `Unable to ${action} service`);
            text(actionMessage, `${label} ${data.saved ? "saved" : "requested"} for ${cards.get(key)?.data.name || key}`);
            await refresh();
        } catch (error) {
            if (operationGeneration === generation) text(actionMessage, error.message);
        } finally {
            if (operationGeneration === generation) {
                pending.delete(key);
                if (snapshot && cards.has(key)) updateCard(cards.get(key), cards.get(key).data, snapshot);
            }
        }
    }
    if (restartAll) restartAll.addEventListener('click', async () => {
        if (!running || connectionLost || !snapshot || restartSending || snapshot.restartActive) return;
        const operationGeneration = generation;
        restartError = ''; restartSending = true; restartAll.disabled = true;
        text(restartStatus,'Requesting graceful node restart...');
        try {
            const response = await fetch('/api/v1/services/all/restart', {
                method:'POST',credentials:'same-origin',headers:{'X-Hub-CSRF':snapshot.csrfToken}
            });
            const data = await response.json();
            if (operationGeneration !== generation) return;
            if (!response.ok) throw new Error(data.error || 'Node restart was rejected');
            await refresh();
        } catch (error) {
            if (operationGeneration === generation) { restartError = error.message; text(restartStatus,restartError); }
        } finally {
            if (operationGeneration === generation) {
                restartSending=false;
                restartAll.disabled=connectionLost || snapshot?.restartActive || !snapshot?.canOperateServices;
            }
        }
    });
    function refresh() {
        if (!running) return Promise.resolve();
        if (request) return request;
        const requestGeneration = generation;
        const abort = new AbortController();
        controller = abort;
        const timeout = window.setTimeout(() => abort.abort(), 5000);
        const activeRequest = (async () => {
            try {
                const response = await fetch("/api/v1/status", {
                    method: "GET", credentials: "same-origin", cache: "no-store", signal: abort.signal
                });
                if (requestGeneration !== generation) return;
                if (response.status === 401) { stop(); callbacks.onUnauthorized?.(); return; }
                if (!response.ok) throw new Error("Status request failed");
                const data = await response.json();
                if (requestGeneration === generation) render(data);
            } catch (error) {
                if (requestGeneration === generation) {
                    markStale();
                }
            } finally { window.clearTimeout(timeout); }
        })();
        request = activeRequest;
        activeRequest.finally(() => {
            if (request === activeRequest) { request = null; controller = null; }
        });
        return activeRequest;
    }
    function start() {
        if (running) return;
        running = true;
        timer = window.setInterval(() => { if (!document.hidden) refresh(); }, 1000);
    }
    function stop() {
        running = false;
        ++generation;
        window.clearInterval(timer);
        controller?.abort();
        timer = request = controller = snapshot = null;
        pending.clear();
        restartSending = false; restartError = '';
        cards.clear();
        grid.replaceChildren();
        text(actionMessage, "");
        if (restartAll) restartAll.disabled = true;
    }
    document.addEventListener("visibilitychange", () => { if (!document.hidden && running) refresh(); });
    return { configure(options) { callbacks = options; }, start, stop, refresh };
})();
