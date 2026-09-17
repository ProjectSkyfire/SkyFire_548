"use strict";

// Independently refreshed sidebar. Consumers never need to poll or rebuild pages.
window.HubStatus = (() => {
    const grid = document.querySelector("#component-grid");
    const summary = document.querySelector("#status-summary");
    const user = document.querySelector("#signed-in-user");
    const updated = document.querySelector("#last-updated");
    const actionMessage = document.querySelector("#service-action-message");
    const online = document.createElement("strong");
    const uptime = document.createElement("span");
    summary.replaceChildren(online, uptime);
    const cards = new Map();
    const pending = new Set();
    let callbacks = {}, snapshot = null, timer = null, request = null, controller = null;
    let running = false, generation = 0;
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
        for (const action of ["start", "stop", "edit"]) {
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
        const active = ["starting", "running", "unresponsive", "stopping"].includes(component.state);
        card.article.className = `component ${component.status}`;
        text(card.title, component.name);
        text(card.state, component.status);
        text(card.detail, component.detail || "");
        card.metrics.hidden = !component.managed;
        card.controls.hidden = !component.managed;
        let metrics = `Uptime ${active ? formatUptime(component.uptimeSeconds) : "—"}`;
        if (isWorld(component)) {
            metrics += component.state === "running" && component.metricsAvailable
                ? ` · ${component.players} players\nCPU ${component.cpuPercent == null ? "—" : component.cpuPercent.toFixed(1) + "%"} · Update ${component.updateTimeMs} ms`
                : "\nPlayers / load unavailable";
            card.metrics.title = "CPU: total logical CPU capacity. Update: world tick processing time, excluding sleep. Metrics expire after 15 seconds without a new tick.";
        }
        text(card.metrics, metrics);
        card.start.disabled = pending.has(component.key) || !data.canOperateServices || !component.enabled || active;
        card.stop.disabled = pending.has(component.key) || !data.canOperateServices || !active || component.state === "stopping";
        card.edit.hidden = !isWorld(component) || data.canSendWorldCommands !== true;
    }
    function render(data) {
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
        if (!running || !snapshot || pending.has(key)) return;
        const operationGeneration = generation;
        pending.add(key);
        updateCard(cards.get(key), cards.get(key).data, snapshot);
        text(actionMessage, `${action === "start" ? "Starting" : "Stopping"} ${cards.get(key).data.name}...`);
        try {
            const response = await fetch(`/api/v1/services/${encodeURIComponent(key)}/${action}`, {
                method: "POST", credentials: "same-origin", headers: { "X-Hub-CSRF": snapshot.csrfToken }
            });
            const data = await response.json();
            if (operationGeneration !== generation) return;
            if (response.status === 401) { stop(); callbacks.onUnauthorized?.(); return; }
            if (!response.ok) throw new Error(data.error || `Unable to ${action} service`);
            text(actionMessage, `${action === "start" ? "Start" : "Stop"} requested for ${cards.get(key)?.data.name || key}`);
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
                if (requestGeneration === generation)
                    text(updated, "Connection lost · retrying (values may be stale)");
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
        cards.clear();
        grid.replaceChildren();
        text(actionMessage, "");
    }
    document.addEventListener("visibilitychange", () => { if (!document.hidden && running) refresh(); });
    return { configure(options) { callbacks = options; }, start, stop, refresh };
})();
