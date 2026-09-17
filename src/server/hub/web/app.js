"use strict";

const refreshIntervalMs = 10000;
const loginView = document.querySelector("#login-view");
const statusView = document.querySelector("#status-view");
const appShell = document.querySelector("#app-shell");
const mainNav = document.querySelector("#main-nav");
const loginForm = document.querySelector("#login-form");
const loginButton = document.querySelector("#login-button");
const loginError = document.querySelector("#login-error");
const logoutButton = document.querySelector("#logout-button");
const signedInUser = document.querySelector("#signed-in-user");
const lastUpdated = document.querySelector("#last-updated");
const statusSummary = document.querySelector("#status-summary");
const componentGrid = document.querySelector("#component-grid");

const worldConsole = document.querySelector("#world-console");
const worldCommandForm = document.querySelector("#world-command-form");
const worldCommandInput = document.querySelector("#world-command");
const worldCommandSend = document.querySelector("#world-command-send");
const worldCommandState = document.querySelector("#world-command-state");
const worldCommandResult = document.querySelector("#world-command-result");
const worldNode = document.querySelector("#world-node");
const nodeSettings = document.querySelector("#world-node-settings");
const nodeForm = document.querySelector("#world-node-form");
const nodeMessage = document.querySelector("#world-node-message");
let currentStatus = null;
const isWorld = (component) => component.isWorld === true || component.key === "world";
let worldCommandSending = false;
let worldCommandAllowed = false;
let refreshTimer = null;
let csrfToken = "";
let canOperateServices = false;

function showLogin(message = "") {
    stopRefresh();
    window.HubAccounts?.reset();
    worldCommandAllowed = false;
    currentStatus = null;
    worldNode.replaceChildren();
    nodeForm.reset();
    nodeMessage.textContent = "";
    worldConsole.hidden = true;
    worldCommandInput.value = "";
    worldCommandResult.textContent = "No command response yet.";
    csrfToken = "";
    canOperateServices = false;
    appShell.hidden = true;
    loginView.hidden = false;
    statusView.hidden = true;
    mainNav.hidden = true;
    loginError.textContent = message;
    loginError.hidden = !message;
    document.querySelector("#password").value = "";
}

function showStatus() {
    appShell.hidden = false;
    loginView.hidden = true;
    statusView.hidden = false;
    mainNav.hidden = false;
}

function startRefresh() {
    stopRefresh();
    refreshTimer = window.setInterval(() => {
        if (!document.hidden)
            loadStatus();
    }, refreshIntervalMs);
}

function stopRefresh() {
    if (refreshTimer !== null) {
        window.clearInterval(refreshTimer);
        refreshTimer = null;
    }
}

function formatUptime(totalSeconds) {
    const days = Math.floor(totalSeconds / 86400);
    const hours = Math.floor((totalSeconds % 86400) / 3600);
    const minutes = Math.floor((totalSeconds % 3600) / 60);
    const values = [];
    if (days)
        values.push(`${days}d`);
    if (hours || days)
        values.push(`${hours}h`);
    values.push(totalSeconds < 60 ? `${Math.floor(totalSeconds)}s` : `${minutes}m`);
    return values.join(" ");
}

function renderStatus(data) {
    currentStatus = data;
    window.HubAccounts?.update(data);
    csrfToken = data.csrfToken || "";
    canOperateServices = data.canOperateServices === true;
    const worlds = data.components.filter(isWorld);
    const selected = worldNode.value;
    worldNode.replaceChildren(...worlds.map((world) => {
        const option = document.createElement("option");
        option.value = world.key;
        option.textContent = `${world.name} (${world.key})`;
        return option;
    }));
    worldNode.value = worlds.some((world) => world.key === selected) ? selected : worlds[0]?.key || "";
    const world = worlds.find((component) => component.key === worldNode.value);
    nodeSettings.hidden = data.canSendWorldCommands !== true;
    worldConsole.hidden = data.canSendWorldCommands !== true;
    worldCommandAllowed = data.canSendWorldCommands === true && !!world && world.enabled &&
        world.state === "running" && world.canSendCommands && !world.commandPending;
    worldCommandSend.disabled = !worldCommandAllowed || worldCommandSending;
    worldCommandState.textContent = world?.commandPending ? "Waiting for worldserver response..." :
        worldCommandAllowed ? "Ready. Commands run with full console access." : "Worldserver must be running under the hub with command support.";
    if (!worldCommandSending)
        worldCommandResult.textContent = world?.commandResult || "No command response yet.";
    signedInUser.textContent = data.username;
    lastUpdated.textContent = `Updated ${new Date().toLocaleTimeString()}`;

    const onlineCount = data.components.filter((component) => component.status === "online").length;
    statusSummary.replaceChildren();
    const summaryStrong = document.createElement("strong");
    summaryStrong.textContent = `${onlineCount} / ${data.components.length} online`;
    const uptime = document.createElement("span");
    uptime.textContent = `Hub uptime ${formatUptime(data.uptimeSeconds)}`;
    statusSummary.append(summaryStrong, uptime);

    componentGrid.replaceChildren(...data.components.map((component) => {
        const article = document.createElement("article");
        article.className = `component ${component.status}`;

        const heading = document.createElement("div");
        heading.className = "component-heading";

        const title = document.createElement("h3");
        title.textContent = component.name;

        const state = document.createElement("span");
        state.className = "status-label";
        state.textContent = component.status;

        const detail = document.createElement("p");
        detail.textContent = component.detail;

        heading.append(title, state);
        article.append(heading, detail);
        if (component.managed) {
            const metrics = document.createElement("p");
            metrics.className = "service-metrics";
            const active = ["starting", "running", "unresponsive", "stopping"].includes(component.state);
            metrics.textContent = `Uptime ${active ? formatUptime(component.uptimeSeconds || 0) : "—"}`;
            if (isWorld(component)) {
                metrics.textContent += component.state === "running" && component.metricsAvailable
                    ? ` · ${component.players} players\nCPU ${component.cpuPercent == null ? "—" : component.cpuPercent.toFixed(1) + "%"} · Update ${component.updateTimeMs} ms`
                    : "\nPlayers / load unavailable";
                metrics.title = "CPU: share of total logical CPU capacity. Update: latest world tick processing time, excluding sleep. Metrics expire after 15 seconds without a new world tick.";
            }
            article.append(metrics);
        }

        if (component.managed) {
            const controls = document.createElement("div");
            controls.className = "service-controls";

            const active = ["starting", "running", "unresponsive", "stopping"].includes(component.state);
            const startButton = document.createElement("button");
            startButton.className = "service-button start";
            startButton.type = "button";
            startButton.textContent = "Start";
            startButton.disabled = !canOperateServices || !component.enabled || active;
            startButton.addEventListener("click", () => operateService(component.key, "start", startButton));

            const stopButton = document.createElement("button");
            stopButton.className = "service-button stop";
            stopButton.type = "button";
            stopButton.textContent = "Stop";
            stopButton.disabled = !canOperateServices || !active || component.state === "stopping";
            stopButton.addEventListener("click", () => operateService(component.key, "stop", stopButton));

            controls.append(startButton, stopButton);
            if (isWorld(component) && data.canSendWorldCommands === true) {
                const edit = document.createElement("button");
                edit.type = "button"; edit.className = "service-button"; edit.textContent = "Edit";
                edit.addEventListener("click", () => {
                    document.querySelector("#status-tab").click();
                    nodeSettings.open = true;
                    for (const [key, value] of Object.entries({ key: component.key, name: component.name,
                        executablePath: component.executablePath, configPath: component.configPath,
                        workingDirectory: component.workingDirectory }))
                        nodeForm.elements.namedItem(key).value = value || "";
                    nodeSettings.scrollIntoView({ block: "nearest", behavior: "smooth" });
                });
                controls.append(edit);
            }
            article.append(controls);
        }
        return article;
    }));
}

worldNode.addEventListener("change", () => { if (currentStatus) renderStatus(currentStatus); });
nodeForm.addEventListener("submit", async (event) => {
    event.preventDefault();
    const data = new FormData(nodeForm);
    const key = data.get("key");
    const button = nodeForm.querySelector("button[type=submit]");
    button.disabled = true;
    try {
        const response = await fetch(`/api/v1/services/${encodeURIComponent(key)}/configure`, {
            method: "POST", credentials: "same-origin",
            headers: { "X-Hub-CSRF": csrfToken, "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams(data)
        });
        const result = await response.json();
        if (!response.ok) throw new Error(result.error || "Unable to save world node.");
        nodeMessage.textContent = result.saved ? "World node saved." : "Save queued. Check the node details before starting it.";
        await loadStatus();
    } catch (error) { nodeMessage.textContent = error.message; }
    finally { button.disabled = false; }
});

worldCommandForm.addEventListener("submit", async (event) => {
    event.preventDefault();
    if (!worldCommandAllowed || worldCommandSending)
        return;
    const command = worldCommandInput.value.trim();
    if (!command || new TextEncoder().encode(command).length > 1024 || /[\x00-\x1f\x7f]/.test(command)) {
        worldCommandResult.textContent = "Enter one command up to 1024 bytes without control characters.";
        return;
    }
    worldCommandSending = true;
    worldNode.disabled = true;
    worldCommandSend.disabled = true;
    worldCommandResult.textContent = "Sending command...";
    try {
        const response = await fetch(`/api/v1/services/${encodeURIComponent(worldNode.value)}/command`, {
            method: "POST",
            credentials: "same-origin",
            headers: { "X-Hub-CSRF": csrfToken, "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams({ command })
        });
        const data = await response.json();
        if (!response.ok)
            throw new Error(data.error || "World command was rejected");
        worldCommandAllowed = false;
        worldCommandResult.textContent = "Command queued. Waiting for worldserver response...";
        window.setTimeout(loadStatus, 500);
    } catch (error) {
        worldCommandResult.textContent = error.message;
    } finally {
        worldCommandSending = false;
        worldNode.disabled = false;
        worldCommandSend.disabled = !worldCommandAllowed;
    }
});

async function operateService(serviceKey, action, button) {
    button.disabled = true;
    const originalText = button.textContent;
    button.textContent = action === "start" ? "Starting..." : "Stopping...";
    try {
        const response = await fetch(`/api/v1/services/${encodeURIComponent(serviceKey)}/${action}`, {
            method: "POST",
            credentials: "same-origin",
            headers: { "X-Hub-CSRF": csrfToken }
        });
        const data = await response.json();
        if (!response.ok)
            throw new Error(data.error || `Unable to ${action} service`);
        lastUpdated.textContent = `${action === "start" ? "Start" : "Stop"} requested`;
        window.setTimeout(loadStatus, 300);
    } catch (error) {
        lastUpdated.textContent = error.message;
        button.disabled = false;
    } finally {
        button.textContent = originalText;
    }
}

async function loadStatus() {
    try {
        const response = await fetch("/api/v1/status", {
            method: "GET",
            credentials: "same-origin",
            cache: "no-store"
        });

        if (response.status === 401) {
            showLogin();
            return;
        }

        if (!response.ok)
            throw new Error("Status request failed");

        const data = await response.json();
        showStatus();
        renderStatus(data);
        if (refreshTimer === null)
            startRefresh();
    } catch (error) {
        lastUpdated.textContent = "Status update failed";
    }
}

loginForm.addEventListener("submit", async (event) => {
    event.preventDefault();
    loginError.hidden = true;
    loginButton.disabled = true;
    loginButton.textContent = "Signing in...";

    try {
        const body = new URLSearchParams(new FormData(loginForm));
        const response = await fetch("/api/v1/session", {
            method: "POST",
            credentials: "same-origin",
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
            body
        });
        const data = await response.json();
        if (!response.ok)
            throw new Error(data.error || "Sign in failed");

        await loadStatus();
    } catch (error) {
        loginError.textContent = error.message;
        loginError.hidden = false;
    } finally {
        loginButton.disabled = false;
        loginButton.textContent = "Sign in";
    }
});

logoutButton.addEventListener("click", async () => {
    try {
        await fetch("/api/v1/logout", {
            method: "POST",
            credentials: "same-origin"
        });
    } finally {
        showLogin();
    }
});

document.addEventListener("visibilitychange", () => {
    if (!document.hidden && appShell.hidden === false)
        loadStatus();
});

loadStatus();
