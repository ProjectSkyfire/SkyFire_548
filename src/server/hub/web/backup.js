/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
window.HubBackupSchedules = class {
    constructor(root, transport) {
        this.root = root; this.transport = transport; this.active = false; this.allowed = false; this.generation = 0;
        this.forms = []; this.loading = false;
    }
    reset() {
        ++this.generation; this.active = false; this.forms = []; this.loading = false; this.root.replaceChildren();
    }
    update(allowed) {
        this.allowed = allowed;
        for (const entry of this.forms) this.permissions(entry);
        if (!this.active) { this.active = true; this.load(); }
    }
    permissions(entry) { for (const input of entry.controls) input.disabled = !this.allowed || entry.saving || input.parentElement?.hidden === true; }
    async load() {
        if (this.loading) return;
        const generation = this.generation; this.loading = true;
        if (!this.message) { this.message = document.createElement('p'); this.message.setAttribute('role','status'); }
        this.root.append(this.message); this.message.textContent = 'Loading backup schedules…';
        try {
            const data = await this.transport();
            if (generation !== this.generation) return;
            this.allowed = this.allowed && data.canEdit;
            this.root.replaceChildren(); this.forms = [];
            const heading = document.createElement('h2'); heading.textContent = 'Backup schedules';
            const explanation = document.createElement('p');
            explanation.textContent = 'Schedule settings are saved in the hub. The backup service is not available yet, so no backups are running. All clock times are UTC.';
            this.root.append(heading, explanation);
            const reload = document.createElement('button'); reload.type = 'button'; reload.textContent = 'Reload saved schedules'; reload.className = 'service-button';
            reload.addEventListener('click', () => this.load()); this.root.append(reload);
            for (const schedule of data.schedules) this.add(schedule);
            this.message.textContent = this.allowed ? 'Choose schedules below; saving does not start a backup.' : 'Administrator permission is required to change schedules.';
            this.root.append(this.message);
        } catch (error) {
            if (generation !== this.generation) return;
            this.message.textContent = error.message;
            if (!this.forms.length) {
                const retry = document.createElement('button'); retry.type = 'button'; retry.textContent = 'Retry loading schedules';
                retry.addEventListener('click', () => this.load()); this.root.append(retry);
            }
        } finally { if (generation === this.generation) this.loading = false; }
    }
    add(schedule) {
        const form = document.createElement('form'); form.className = 'account-form backup-schedule';
        const title = document.createElement('h3'); title.textContent = {auth:'Authentication',characters:'Characters',world:'World',hub:'Hub'}[schedule.target]; form.append(title);
        const controls = [];
        const field = (labelText, type, value, options) => {
            const label = document.createElement('label'); label.textContent = labelText;
            const input = document.createElement(options ? 'select' : 'input');
            if (options) for (const [key, text] of options) { const option = document.createElement('option'); option.value = key; option.textContent = text; input.append(option); }
            else input.type = type;
            input.value = String(value); label.append(input); form.append(label); controls.push(input); return input;
        };
        const enabled = field('Use this schedule when backups become available', '', schedule.enabled, [['0','Disabled'],['1','Enabled — awaiting backup service']]);
        const mode = field('Frequency', '', schedule.mode, [['interval','Every N minutes'],['daily','Daily'],['weekly','Weekly']]);
        const interval = field('Interval in minutes (15–10080)', 'number', schedule.intervalMinutes); interval.min = '15'; interval.max = '10080'; interval.step = '1';
        const time = field('Time (UTC)', 'time', String(Math.floor(schedule.minuteOfDay/60)).padStart(2,'0')+':'+String(schedule.minuteOfDay%60).padStart(2,'0')); time.step = '60';
        const weekday = field('Day (UTC)', '', schedule.weekday, ['Monday','Tuesday','Wednesday','Thursday','Friday','Saturday','Sunday'].map((day,index)=>[String(index),day]));
        const modeChanged = () => {
            interval.parentElement.hidden = mode.value !== 'interval'; interval.required = mode.value === 'interval';
            time.parentElement.hidden = mode.value === 'interval'; time.required = mode.value !== 'interval';
            weekday.parentElement.hidden = mode.value !== 'weekly';
        };
        mode.addEventListener('change', () => { modeChanged(); this.permissions(entry); }); modeChanged();
        const save = document.createElement('button'); save.type = 'submit'; save.textContent = 'Save schedule'; save.className = 'primary-button'; controls.push(save);
        const message = document.createElement('p'); message.setAttribute('role','status');
        form.append(save,message); this.root.append(form);
        const entry = {controls, saving:false}; this.forms.push(entry); this.permissions(entry);
        form.addEventListener('submit', async event => {
            event.preventDefault(); if (!this.allowed || entry.saving) return;
            const generation = this.generation;
            const id = Array.from(crypto.getRandomValues(new Uint8Array(16)), byte=>byte.toString(16).padStart(2,'0')).join('');
            const parts = time.value.split(':').map(Number);
            const value = { id, target:schedule.target, enabled:Number(enabled.value), mode:mode.value,
                intervalMinutes:mode.value === 'interval' ? Number(interval.value) : 60,
                minuteOfDay:mode.value === 'interval' ? 0 : parts[0]*60+parts[1], weekday:mode.value === 'weekly' ? Number(weekday.value) : 0, revision:schedule.revision };
            entry.saving = true; this.permissions(entry); message.textContent = 'Saving…';
            try {
                const data = await this.transport(value);
                if (generation !== this.generation) return;
                schedule.revision = data.schedules.find(item=>item.target===schedule.target).revision;
                message.textContent = 'Schedule saved. Awaiting the backup service; no backups are running yet.';
            } catch (error) { if (generation === this.generation) message.textContent = error.message + ' Reload saved schedules before retrying.'; }
            finally { entry.saving = false; if (generation === this.generation) this.permissions(entry); }
        });
    }
};
