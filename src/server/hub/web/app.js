"use strict";

const loginView = document.querySelector("#login-view");
const statusView = document.querySelector("#status-view");
const appShell = document.querySelector("#app-shell");
const mainNav = document.querySelector("#main-nav");
const loginForm = document.querySelector("#login-form");
const loginButton = document.querySelector("#login-button");
const loginError = document.querySelector("#login-error");
const logoutButton = document.querySelector("#logout-button");

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
window.hubPages = new window.HubPageNavigation();
const healthDashboard = new window.HubHealthDashboard(document.querySelector('#health-dashboard'));
let currentStatus = null;
const isWorld = (component) => component.isWorld === true || component.key === "world";
let worldCommandSending = false;
let worldCommandAllowed = false;
let statusSubscription = "";
let worldOptionsSignature = "";
let csrfToken = "";
const backupSchedules = new window.HubBackupSchedules(document.querySelector('#backup-schedules'), async (value, section = 'schedules') => {
    const response = await fetch('/api/v1/backup/' + section, {
        method:value ? 'POST' : 'GET', credentials:'same-origin', cache:'no-store',
        headers:value ? {'Content-Type':'application/x-www-form-urlencoded','X-Hub-CSRF':csrfToken} : {},
        body:value ? new URLSearchParams(value) : undefined, signal:AbortSignal.timeout(8000)
    });
    const data = await response.json();
    if (!response.ok) throw new Error(data.error || 'Backup schedule request failed');
    return data;
});

function showLogin(message = "") {
    window.HubStatus.stop();
    statusSubscription = "";
    worldOptionsSignature = "";
    window.HubAccounts?.reset();
    backupSchedules.reset();
    window.HubCertificates?.reset();
    healthDashboard.reset();
    window.hubPages.reset();
    worldCommandAllowed = false;
    currentStatus = null;
    worldNode.replaceChildren();
    nodeForm.reset();
    nodeMessage.textContent = "";
    worldConsole.hidden = true;
    worldCommandInput.value = "";
    worldCommandResult.textContent = "No command response yet.";
    csrfToken = "";
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
    window.hubPages.reset();
    mainNav.hidden = false;
}

function renderStatus(data) {
    currentStatus = data;
    document.querySelector("#status-tab").hidden = data.canSendWorldCommands !== true && data.canOperateServices !== true;
    window.HubAccounts?.update(data);
    csrfToken = data.csrfToken || "";
    window.HubCertificates?.configure(data.canSendWorldCommands === true, csrfToken);
    backupSchedules.update(data.canOperateServices === true || data.canSendWorldCommands === true);
    const worlds = data.components.filter(isWorld);
    const selected = worldNode.value;
    const signature = JSON.stringify(worlds.map((world) => [world.key, world.name]));
    if (worldOptionsSignature !== signature) {
        worldOptionsSignature = signature;
        worldNode.replaceChildren(...worlds.map((world) => {
            const option = document.createElement("option");
            option.value = world.key;
            option.textContent = `${world.name} (${world.key})`;
            return option;
        }));
    }
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

// Server Status owns polling and sidebar rendering. Page consumers receive only
// changed control data; metric-only ticks never touch account forms or the console.
function acceptStatus(data) {
    if (backupSchedules.manual && Date.now() - backupSchedules.jobLastRead >= 3000) backupSchedules.loadJobs();
    if (appShell.hidden) showStatus();
    healthDashboard.update({hubUptime:data.uptimeSeconds||0,services:data.components.filter(item=>item.managed || item.mapserver || item.characterserver || item.chatserver).map(item=>({
        ...item,world:isWorld(item),cpu:item.cpuPercent,update:item.updateTimeMs}))});
    const controlData = {
        csrfToken: data.csrfToken, canSendWorldCommands: data.canSendWorldCommands,
        accountsEnabled: data.accountsEnabled, canOperateServices: data.canOperateServices,
        components: data.components.filter(isWorld).map(({key, name, isWorld, enabled, state,
            canSendCommands, commandPending, commandResult}) =>
            ({key, name, isWorld, enabled, state, canSendCommands, commandPending, commandResult}))
    };
    const signature = JSON.stringify(controlData);
    if (signature !== statusSubscription) {
        statusSubscription = signature;
        renderStatus(controlData);
    }
}

function editWorldNode(component) {
    document.querySelector("#status-tab").click();
    nodeSettings.open = true;
    for (const [key, value] of Object.entries({ key: component.key, name: component.name,
        executablePath: component.executablePath, configPath: component.configPath,
        workingDirectory: component.workingDirectory }))
        nodeForm.elements.namedItem(key).value = value || "";
    nodeSettings.scrollIntoView({ block: "nearest", behavior: "smooth" });
}

function loadStatus() {
    return window.HubStatus.refresh();
}
window.HubStatus.configure({ onStatus: acceptStatus, onUnauthorized: showLogin, onStale: () => healthDashboard.stale(), onEdit: editWorldNode });

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

        window.HubStatus.start();
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
    showLogin();
    try {
        await fetch("/api/v1/logout", {
            method: "POST",
            credentials: "same-origin"
        });
    } finally {
        showLogin();
    }
});

window.HubStatus.start();
loadStatus();
