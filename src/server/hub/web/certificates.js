/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
(() => {
    const tab = document.querySelector('#certificates-tab');
    const list = document.querySelector('#certificates-list');
    const message = document.querySelector('#certificates-message');
    const token = document.querySelector('#certificates-token');
    const form = document.querySelector('#certificates-enroll');
    let csrf = '', generation = 0, busy = false, clearToken;
    async function request(values) {
        const response = await fetch('/api/v1/certificates', {
            method: values ? 'POST' : 'GET', credentials:'same-origin', cache:'no-store',
            headers:values ? {'Content-Type':'application/x-www-form-urlencoded', 'X-Hub-CSRF':csrf} : {},
            body:values ? new URLSearchParams(values) : undefined, signal:AbortSignal.timeout(15000)
        });
        const data = await response.json();
        if (!response.ok) throw new Error(data.error || 'Certificate request failed');
        return data;
    }
    function render(data) {
        list.replaceChildren();
        form.hidden = !data.enabled;
        if (!data.enabled) { message.textContent = 'Enable Hub.PKI in the hub configuration to manage certificates.'; return; }
        for (const node of data.nodes) {
            const row = document.createElement('section'); row.className = 'account-panel';
            const title = document.createElement('h2'); title.textContent = node.node;
            const detail = document.createElement('p');
            detail.textContent = `${['','Auth','World','Map','Character','Chat'][node.role]} · ${node.local ? 'Managed' : 'Remote'} · ${node.revoked ? 'Revoked' : node.expires * 1000 <= Date.now() ? 'Expired' : 'Valid'} · Expires ${node.expires ? new Date(node.expires * 1000).toLocaleString() : '—'} · ${node.names}`;
            row.append(title, detail);
            if (node.node !== 'skyfire-hub-authority') for (const action of node.local ? ['revoke','regenerate'] : ['revoke','token']) {
                const button = document.createElement('button'); button.type = 'button'; button.textContent = action === 'revoke' ? 'Revoke' : action === 'token' ? 'Generate replacement token' : 'Regenerate';
                button.addEventListener('click', () => {
                    if (action === 'token') { run({action, node:node.node, role:String(node.role), names:node.names}); return; }
                    if (confirm(`${button.textContent} ${node.node}? Existing connections using its old certificate will be rejected.`))
                        run({action, node:node.node});
                });
                row.append(button);
            }
            list.append(row);
        }
    }
    async function run(values) {
        if (busy || !csrf) return;
        busy = true; const current = generation;
        if (values?.action === 'token') { clearTimeout(clearToken); token.textContent = ''; }
        message.textContent = 'Working…';
        try {
            const data = await request(values);
            if (current !== generation) return;
            message.textContent = values ? 'Certificate action completed.' : '';
            if (data.token) {
                token.textContent = data.token; clearTimeout(clearToken);
                clearToken = setTimeout(() => { token.textContent = ''; }, 600000);
            } else render(data);
        } catch (error) { if (current === generation) message.textContent = error.message; }
        finally { if (current === generation) busy = false; }
    }
    form.addEventListener('submit', event => { event.preventDefault(); run({action:'token', ...Object.fromEntries(new FormData(form))}); });
    tab.addEventListener('click', () => run());
    document.querySelector('#certificates-refresh').addEventListener('click', () => run());
    window.HubCertificates = {
        configure(allowed, value) { tab.hidden = !allowed; csrf = allowed ? value : ''; if (!allowed) this.reset(); },
        reset() { ++generation; busy = false; csrf = ''; clearTimeout(clearToken); token.textContent = ''; list.replaceChildren(); message.textContent = ''; form.reset(); tab.hidden = true; }
    };
})();
