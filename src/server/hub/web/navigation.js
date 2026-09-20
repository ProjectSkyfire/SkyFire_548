/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
window.HubPageNavigation = class {
    constructor() {
        this.current = 'health';
        this.tabs = [...document.querySelectorAll('[data-page]')];
        this.views = [...document.querySelectorAll('[data-page-view]')];
        for (const tab of this.tabs) tab.addEventListener('click', () => this.select(tab.dataset.page));
        this.refresh();
    }
    select(page) {
        const tab = this.tabs.find(item => item.dataset.page === page);
        if (!tab || tab.hidden) return;
        this.current = page; this.refresh();
    }
    refresh() {
        if (this.tabs.find(tab => tab.dataset.page === this.current)?.hidden) this.current = 'health';
        for (const tab of this.tabs) {
            const active = tab.dataset.page === this.current;
            tab.classList.toggle('active', active);
            if (active) tab.setAttribute('aria-current','page'); else tab.removeAttribute('aria-current');
        }
        for (const view of this.views) view.hidden = view.dataset.pageView !== this.current;
    }
    reset() { this.current = 'health'; this.refresh(); }
};

// Keep layout independent of the live-status renderer and page navigation.
(() => {
    const shell = document.querySelector('#app-shell');
    const sidebar = document.querySelector('#server-sidebar');
    const toggle = document.querySelector('#sidebar-toggle');
    const handle = document.querySelector('#sidebar-resize');
    if (!shell || !sidebar || !toggle || !handle) return;
    const key = 'skyfire.hub.sidebar';
    let width = 292, folded = false, drag = null;
    try {
        const saved = JSON.parse(localStorage.getItem(key));
        if (Number.isFinite(saved?.width)) width = Math.max(240, Math.min(560, saved.width));
        folded = saved?.folded === true;
    } catch (_) { /* Storage can be unavailable in private/restricted sessions. */ }
    function save() {
        try { localStorage.setItem(key, JSON.stringify({width, folded})); } catch (_) { }
    }
    function render() {
        const maximum = Math.max(240, Math.min(560, (shell.clientWidth || window.innerWidth - 36) - 388));
        const actual = Math.min(width, maximum);
        shell.style.setProperty('--sidebar-width', `${actual}px`);
        shell.classList.toggle('status-folded', folded);
        sidebar.hidden = folded;
        handle.hidden = folded;
        toggle.setAttribute('aria-expanded', String(!folded));
        toggle.title = folded ? 'Open server status' : 'Fold server status';
        document.querySelector('#sidebar-toggle-icon').textContent = folded ? '›' : '‹';
        handle.setAttribute('aria-valuemax', String(maximum));
        handle.setAttribute('aria-valuenow', String(actual));
    }
    function finish() {
        if (!drag) return;
        const id = drag.id; drag = null;
        if (handle.hasPointerCapture(id)) handle.releasePointerCapture(id);
        document.body.classList.remove('sidebar-dragging');
        save();
    }
    toggle.addEventListener('click', () => { finish(); folded = !folded; render(); save(); });
    handle.addEventListener('pointerdown', event => {
        if (event.button !== 0) return;
        event.preventDefault();
        drag = {id:event.pointerId, x:event.clientX, width:Number(handle.getAttribute('aria-valuenow'))};
        handle.setPointerCapture(event.pointerId);
        document.body.classList.add('sidebar-dragging');
    });
    handle.addEventListener('pointermove', event => {
        if (!drag || event.pointerId !== drag.id) return;
        width = Math.max(240, Math.min(Number(handle.getAttribute('aria-valuemax')), drag.width + event.clientX - drag.x));
        render();
    });
    for (const event of ['pointerup','pointercancel','lostpointercapture']) handle.addEventListener(event, finish);
    handle.addEventListener('keydown', event => {
        if (!['ArrowLeft','ArrowRight','Home','End'].includes(event.key)) return;
        event.preventDefault();
        const max = Number(handle.getAttribute('aria-valuemax'));
        width = event.key === 'Home' ? 240 : event.key === 'End' ? max : Math.max(240, Math.min(max, Number(handle.getAttribute('aria-valuenow')) + (event.key === 'ArrowRight' ? 20 : -20)));
        render(); save();
    });
    window.addEventListener('resize', () => { finish(); render(); });
    new ResizeObserver(render).observe(shell);
    render();
})();
