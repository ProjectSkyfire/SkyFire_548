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
