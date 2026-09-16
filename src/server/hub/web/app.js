"use strict";

const refreshIntervalMs = 10000;
const loginView = document.querySelector("#login-view");
const statusView = document.querySelector("#status-view");
const mainNav = document.querySelector("#main-nav");
const loginForm = document.querySelector("#login-form");
const loginButton = document.querySelector("#login-button");
const loginError = document.querySelector("#login-error");
const logoutButton = document.querySelector("#logout-button");
const signedInUser = document.querySelector("#signed-in-user");
const lastUpdated = document.querySelector("#last-updated");
const statusSummary = document.querySelector("#status-summary");
const componentGrid = document.querySelector("#component-grid");

let refreshTimer = null;
let csrfToken = "";
let canOperateServices = false;

function showLogin(message = "") {
    stopRefresh();
    csrfToken = "";
    canOperateServices = false;
    loginView.hidden = false;
    statusView.hidden = true;
    mainNav.hidden = true;
    loginError.textContent = message;
    loginError.hidden = !message;
    document.querySelector("#password").value = "";
}

function showStatus() {
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
    values.push(`${minutes}m`);
    return values.join(" ");
}

function renderStatus(data) {
    csrfToken = data.csrfToken || "";
    canOperateServices = data.canOperateServices === true;
    signedInUser.textContent = data.username;
    lastUpdated.textContent = `Updated ${new Date().toLocaleTimeString()}`;

    const onlineCount = data.components.filter((component) => component.status === "online").length;
    statusSummary.replaceChildren();
    const summaryStrong = document.createElement("strong");
    summaryStrong.textContent = `${onlineCount} of ${data.components.length} components online`;
    statusSummary.append(summaryStrong, ` | Hub uptime ${formatUptime(data.uptimeSeconds)}`);

    componentGrid.replaceChildren(...data.components.map((component) => {
        const article = document.createElement("article");
        article.className = `component ${component.status}`;

        const heading = document.createElement("div");
        heading.className = "component-heading";

        const title = document.createElement("h2");
        title.textContent = component.name;

        const state = document.createElement("span");
        state.className = "status-label";
        state.textContent = component.status;

        const detail = document.createElement("p");
        detail.textContent = component.detail;

        heading.append(title, state);
        article.append(heading, detail);

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
            article.append(controls);
        }
        return article;
    }));
}

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
    if (!document.hidden && statusView.hidden === false)
        loadStatus();
});

loadStatus();
