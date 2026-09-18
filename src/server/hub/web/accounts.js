"use strict";

(() => {
    const view = document.querySelector("#accounts-view");
    const statusTab = document.querySelector("#status-tab");
    const accountsTab = document.querySelector("#accounts-tab");
    const message = document.querySelector("#accounts-message");
    const workspace = document.querySelector("#accounts-workspace");
    let csrf = "", permitted = false, enabled = false, active = false, busy = false;
    let selectedId = 0, searchNext = 0, permissionNext = 0, ipNext = "", generation = 0;

    const byId = (id) => document.getElementById(id);
    const field = (form, name) => form.elements.namedItem(name);
    const values = (form) => Object.fromEntries(new FormData(form));
    function announce(text, error = false) { message.textContent = text; message.classList.toggle("form-error", error); }
    function showView() {
        view.hidden = !active || !permitted;
        document.querySelector("#status-view").hidden = active && permitted;
        statusTab.classList.toggle("active", !active);
        accountsTab.classList.toggle("active", active);
        statusTab.setAttribute("aria-current", active ? "false" : "page");
        accountsTab.setAttribute("aria-current", active ? "page" : "false");
    }
    function setBusy(value) {
        busy = value;
        workspace.querySelectorAll("[data-mutation]").forEach((button) => { button.disabled = value || !enabled || !permitted; });
    }
    async function request(action, data = {}) {
        if (!permitted || !enabled) throw new Error("Account administration is unavailable.");
        const requestGeneration = generation;
        const response = await fetch(`/api/v1/accounts/${action}`, {
            method: "POST", credentials: "same-origin", cache: "no-store",
            headers: { "X-Hub-CSRF": csrf, "Content-Type": "application/x-www-form-urlencoded" },
            body: new URLSearchParams(data)
        });
        const result = await response.json();
        if (requestGeneration !== generation) throw new Error("Session changed.");
        if (!response.ok) throw new Error(result.error || "Account request failed.");
        return result;
    }
    async function change(action, data) {
        if (busy) return null;
        setBusy(true);
        const currentGeneration = generation;
        announce("Submitting change through the hub...");
        try {
            let result = await request(action, data);
            const job = result.job;
            if (job) {
                const deadline = Date.now() + 120000;
                do {
                    if (generation !== currentGeneration) return null;
                    await new Promise((resolve) => window.setTimeout(resolve, 500));
                    result = await request("result", { job });
                    if (!result.pending) break;
                    if (Date.now() >= deadline) throw new Error("Still awaiting the server. Check the account state before retrying; the change may complete later.");
                } while (true);
            }
            if (generation !== currentGeneration) return null;
            announce(result.message || "Change saved.");
            return result;
        } catch (error) {
            if (generation === currentGeneration) announce(error.message, true);
            return null;
        } finally {
            if (generation === currentGeneration) setBusy(false);
        }
    }
    function table(target, headers, rows) {
        target.replaceChildren();
        const element = document.createElement("table"), head = document.createElement("thead"), tr = document.createElement("tr");
        for (const name of headers) { const th = document.createElement("th"); th.textContent = name; th.scope = "col"; tr.append(th); }
        head.append(tr); element.append(head);
        const body = document.createElement("tbody");
        for (const row of rows) {
            const line = document.createElement("tr");
            for (const value of row) { const td = document.createElement("td"); if (value instanceof Node) td.append(value); else td.textContent = String(value); line.append(td); }
            body.append(line);
        }
        element.append(body); target.append(element);
        if (!rows.length) { const empty = document.createElement("p"); empty.textContent = "No matching records."; target.append(empty); }
    }
    function button(label, handler) {
        const result = document.createElement("button"); result.type = "button"; result.className = "service-button";
        result.textContent = label; result.addEventListener("click", handler); return result;
    }
    const stamp = (seconds) => seconds > 0 ? new Date(seconds * 1000).toLocaleString() : "None";
    const expiry = (row) => row.start === row.end ? "Permanent" : stamp(row.end);
    function readAction(callback) { return async (event) => { event?.preventDefault(); try { await callback(); } catch (error) { announce(error.message, true); } }; }

    async function search(after = 0) {
        const data = await request("search", { query: field(byId("account-search"), "query").value, after });
        searchNext = data.next; byId("account-next").disabled = !searchNext;
        table(byId("account-results"), ["ID", "Account", "Email", "Last IP", "Status", ""], data.items.map((item) => [
            item.id, item.name, item.email, item.ip, item.banned ? "Banned" : item.online ? "Online" : "Offline",
            button("Edit", readAction(() => select(item.id)))
        ]));
    }
    async function select(id) {
        const data = await request("detail", { id }); selectedId = data.id;
        byId("account-editor").hidden = false;
        byId("account-selected").textContent = `${data.username} · Account ${data.id}`;
        const form = byId("account-profile");
        field(form, "username").value = data.username; field(form, "email").value = data.email;
        field(form, "expansion").value = String(data.expansion); field(form, "password").value = "";
        byId("account-current-state").textContent = `${data.banned ? "Banned" : "Not banned"} · Last IP: ${data.ip} · ` +
            (data.muteUntil < 0 ? `Muted for ${Math.abs(data.muteUntil) / 60} minutes on next login` : data.muteUntil > Date.now() / 1000 ? `Muted until ${stamp(data.muteUntil)}: ${data.muteReason}` : "Not muted");
        table(byId("account-gm-current"), ["Realm", "GM level"], data.gm.map((item) => [item.realm === -1 ? "All realms" : item.realm, item.level]));
        table(byId("account-rbac-current"), ["Realm", "Permission", "Name", "Override"], data.rbac.map((item) => [item.realm === -1 ? "All realms" : item.realm, item.permission, item.name, item.granted ? "Grant" : "Deny"]));
        table(byId("account-ban-history"), ["Banned at", "Expires", "By", "Reason"], data.bans.map((item) => [stamp(item.start), expiry(item), item.by, item.reason]));
    }
    async function permissions(after = 0) {
        const data = await request("permissions", { query: field(byId("permission-search"), "query").value, after });
        permissionNext = data.next; byId("permission-next").disabled = !permissionNext;
        table(byId("permission-results"), ["ID", "Permission", ""], data.items.map((item) => [item.id, item.name,
            button("Use", () => { field(byId("account-rbac"), "permission").value = String(item.id); })]));
    }
    async function ipList(after = "") {
        const data = await request("ip-list", { query: field(byId("ip-search"), "query").value, after });
        ipNext = data.next; byId("ip-next").disabled = !ipNext;
        table(byId("ip-results"), ["IP", "Expires", "By", "Reason", ""], data.items.map((item) => [item.ip, expiry(item), item.by, item.reason,
            button("Use", () => { field(byId("ip-ban-form"), "ip").value = item.ip; })]));
    }
    async function loadRealms() {
        const data = await request("realms");
        workspace.querySelectorAll("select[name=realm]").forEach((select) => {
            select.replaceChildren(...data.items.map((item) => { const option = document.createElement("option"); option.value = String(item.id); option.textContent = `${item.name} (${item.id})`; return option; }));
        });
    }
    statusTab.addEventListener("click", () => { active = false; showView(); });
    accountsTab.addEventListener("click", readAction(async () => {
        active = true; showView();
        if (enabled) await Promise.all([search(), loadRealms(), ipList()]);
    }));
    byId("account-search").addEventListener("submit", readAction(() => search()));
    byId("account-next").addEventListener("click", readAction(() => search(searchNext)));
    byId("permission-search").addEventListener("submit", readAction(() => permissions()));
    byId("permission-next").addEventListener("click", readAction(() => permissions(permissionNext)));
    byId("ip-search").addEventListener("submit", readAction(() => ipList()));
    byId("ip-next").addEventListener("click", readAction(() => ipList(ipNext)));
    byId("account-refresh").addEventListener("click", readAction(() => selectedId ? select(selectedId) : Promise.resolve()));

    byId("account-create").addEventListener("submit", async (event) => {
        event.preventDefault(); if (busy) return; const form = event.currentTarget, data = values(form);
        field(form, "password").value = "";
        const result = await change("create", data); data.password = "";
        if (result) await readAction(async () => { form.reset(); await search(); if (result.id) await select(result.id); })();
    });
    const actions = { "account-profile": "update", "account-ban": "ban", "account-mute": "mute", "account-gm": "gm", "account-rbac": "rbac" };
    for (const [id, action] of Object.entries(actions)) byId(id).addEventListener("submit", async (event) => {
        event.preventDefault(); if (busy || !selectedId) return;
        const data = { ...values(event.currentTarget), id: selectedId }, target = selectedId;
        if (action === "update") field(event.currentTarget, "password").value = "";
        const result = await change(action, data); if (data.password) data.password = "";
        if (result && selectedId === target) await readAction(() => select(target))();
    });
    for (const action of ["unban", "unmute"]) byId(`account-${action}`).addEventListener("click", async () => {
        const target = selectedId; if (!target) return;
        if (await change(action, { id: target }) && selectedId === target) await readAction(() => select(target))();
    });
    byId("ip-ban-form").addEventListener("submit", async (event) => {
        event.preventDefault(); if (await change("ip-ban", values(event.currentTarget))) await readAction(() => ipList())();
    });
    byId("ip-unban").addEventListener("click", async () => {
        if (await change("ip-unban", { ip: field(byId("ip-ban-form"), "ip").value })) await readAction(() => ipList())();
    });
    window.HubAccounts = {
        update(data) {
            csrf = data.csrfToken || ""; permitted = data.canSendWorldCommands === true; enabled = data.accountsEnabled === true;
            accountsTab.hidden = !permitted; workspace.hidden = !enabled;
            byId("accounts-disabled").hidden = enabled;
            const worlds = data.components.filter((item) => item.isWorld === true || item.key === "world");
            const world = worlds.find((item) => item.state === "running") ||
                worlds.find((item) => ["starting", "stopping", "unresponsive"].includes(item.state));
            byId("accounts-routing").textContent = world?.state === "running" ? "Changes are executed by worldserver through the hub." :
                ["starting", "stopping", "unresponsive"].includes(world?.state) ? "Worldserver is transitioning. Account changes are temporarily unavailable." :
                "Worldserver is stopped. The hub handles account changes directly.";
            setBusy(busy); showView();
        },
        reset() {
            generation++; csrf = ""; permitted = false; enabled = false; active = false; busy = false; selectedId = 0;
            view.hidden = true; accountsTab.hidden = true; byId("account-editor").hidden = true;
            workspace.querySelectorAll("form").forEach((form) => form.reset());
            workspace.querySelectorAll(".account-table").forEach((target) => target.replaceChildren());
            announce("");
        }
    };
})();
