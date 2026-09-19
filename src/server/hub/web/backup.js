/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
window.HubBackupSchedules = class {
    constructor(root, transport) {
        this.root = root; this.transport = transport; this.active = false; this.allowed = false; this.generation = 0;
        this.forms = []; this.loading = false; this.jobLoading = false; this.jobLastRead = 0; this.jobSending = false;
    }
    reset() {
        ++this.generation; this.jobLoading = false; this.jobLastRead = 0; this.jobSending = false; this.manual = null; this.active = false; this.forms = []; this.loading = false; this.jobLoading = false; this.jobLastRead = 0; this.jobSending = false; this.root.replaceChildren();
    }
    update(allowed) {
        this.allowed = allowed;
        for (const entry of this.forms) this.permissions(entry);
        if (!this.active) { this.active = true; this.load(); }
        if (this.manual && Date.now() - this.jobLastRead >= 3000) this.loadJobs();
        this.jobPermissions();
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
            explanation.textContent = 'The backup worker executes enabled schedules. All schedule times are UTC. Missed occurrences are combined into one run. MyISAM backups wait for game services to be stopped gracefully.';
            this.createManual();
            this.root.append(heading, explanation);
            const reload = document.createElement('button'); reload.type = 'button'; reload.textContent = 'Reload saved schedules'; reload.className = 'service-button';
            reload.addEventListener('click', () => this.load()); this.root.append(reload);
            for (const schedule of data.schedules) this.add(schedule);
            this.message.textContent = this.allowed ? 'Save a schedule to set its next occurrence.' : 'Administrator permission is required to change schedules.';
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
    createManual() {
        const section = document.createElement('section'); section.className = 'manual-backups';
        const heading = document.createElement('h2'); heading.textContent = 'Manual backup';
        const label = document.createElement('label'); label.textContent = 'Database';
        const select = document.createElement('select'); select.setAttribute('aria-label','Backup database');
        for (const target of ['auth','characters','world','hub']) {
            const option = document.createElement('option'); option.value = target; option.textContent = target; select.append(option);
        }
        label.append(select);
        const button = document.createElement('button'); button.type = 'button'; button.textContent = 'Backup now'; button.className = 'primary-button';
        const status = document.createElement('p'); status.setAttribute('role','status');
        const result = document.createElement('p'); result.setAttribute('role','status');
        const storage = document.createElement('p'); storage.setAttribute('aria-label','Backup volume usage');
        const automation = document.createElement('p'); automation.setAttribute('aria-label','Backup automation');
        const restoreLabel = document.createElement('label'); restoreLabel.textContent = 'Recovery point';
        const restoreSelect = document.createElement('select'); restoreSelect.setAttribute('aria-label','Restore recovery point'); restoreLabel.append(restoreSelect);
        const confirmLabel = document.createElement('label'); confirmLabel.textContent = 'Type RESTORE followed by the database target to replace live data';
        const confirm = document.createElement('input'); confirm.setAttribute('aria-label','Restore confirmation'); confirmLabel.append(confirm);
        const restore = document.createElement('button'); restore.type='button'; restore.textContent='Restore live database'; restore.className='service-button';
        const release = document.createElement('button'); release.type='button'; release.textContent='End recovery maintenance'; release.className='service-button';
        const restoreNote = document.createElement('p'); restoreNote.textContent='Gracefully stop all game services before restoring. A staging check and fresh rollback backup precede replacement. Hub database recovery is offline. Rollback archives are protected from automatic deletion.';
        const historyHeading=document.createElement('h3'); historyHeading.textContent='Latest 30 jobs';
        const history = document.createElement('ol'); history.setAttribute('aria-label','Backup jobs');
        section.append(heading,label,button,status,storage,automation,restoreLabel,confirmLabel,restore,release,restoreNote,result,historyHeading,history); this.root.append(section);
        this.manual = {select,button,status,result,history,storage,automation,restoreSelect,confirm,restore,release}; this.restoreSignature = null; this.jobData = null;
        select.addEventListener('change',()=>this.jobPermissions());
        button.addEventListener('click',()=>this.startBackup());
        confirm.addEventListener('input',()=>this.jobPermissions()); restoreSelect.addEventListener('change',()=>this.jobPermissions());
        restore.addEventListener('click',()=>this.startBackup('restore')); release.addEventListener('click',()=>this.startBackup('release'));
        this.jobLastRead = 0; this.loadJobs(); this.jobPermissions();
    }
    jobPermissions() {
        if (!this.manual) return;
        const data = this.jobData;
        const active = data?.jobs?.some(job=>['queued','running'].includes(job.state));
        this.manual.button.disabled = !this.allowed || !data?.canRun || !data.available || this.jobSending || active || data.maintenance ||
            !(data.targets || '').split(',').includes(this.manual.select.value);
        this.manual.select.disabled = this.jobSending;
        const source=data?.jobs?.find(job=>job.id===this.manual.restoreSelect.value);
        this.manual.restore.disabled = !this.allowed || !data?.canRestore || !data.available || data.maintenance || active || this.jobSending ||
            !source || this.manual.confirm.value !== 'RESTORE ' + source.target;
        this.manual.restoreSelect.disabled = this.jobSending; this.manual.confirm.disabled = this.jobSending;
        this.manual.release.hidden = !data?.maintenance;
        this.manual.release.disabled = !this.allowed || !data?.canRun || !data?.recoverySafe || active || this.jobSending;
        for (const button of this.pinButtons || []) button.disabled = !this.allowed || !data?.canRun || this.jobSending;
    }
    renderJobs(data) {
        if (!this.manual) return;
        this.jobData = data;
        this.manual.status.textContent = data.available ? 'Backup worker online. Dumps are checksum checked; restore verification is separate.' : 'Backup worker offline. Start the configured backup worker to run backups.';
        const size=value=>{ let unit=0; value=Number(value)||0; while(value>=1024 && unit<4) { value/=1024; unit++; } return value.toFixed(unit ? 2 : 0)+' '+['B','KiB','MiB','GiB','TiB'][unit]; };
        this.manual.storage.textContent = `Backup volume: ${size(data.storageFree||0)} free of ${size(data.storageTotal||0)}. ${data.archiveCount||0} archives using ${size(data.archiveBytes||0)} (including rollback copies). ${data.maintenance ? 'Recovery maintenance is active.' : ''}`;
        let automation = {}; try { automation = JSON.parse(data.automation || '{}'); } catch (_) {}
        const date = value => value ? new Date(value*1000).toLocaleString() : 'none';
        this.manual.automation.textContent = `Retention: ${automation.retentionDays ? automation.retentionDays+' days' : 'disabled or worker policy unavailable'}. Pinned archives, rollback copies and the newest verified recovery point per database are protected. ` +
            (automation.schedules || []).map(item=>`${item.target}: ${item.status}; next ${date(item.nextRun)}; last backup ${date(item.backupAt)}; last verified restore ${date(item.verifiedAt)}.`).join(' ');
        const sources=data.jobs.filter(job=>job.kind==='backup' && job.state==='completed' && job.target!=='hub');
        const signature=JSON.stringify(sources.map(job=>job.id));
        if (signature!==this.restoreSignature) {
            const selected=this.manual.restoreSelect.value; this.restoreSignature=signature;
            const placeholder=document.createElement('option'); placeholder.value=''; placeholder.textContent='Select a recovery point';
            this.manual.restoreSelect.replaceChildren(placeholder,...sources.map(job=>{
                const option=document.createElement('option'); option.value=job.id; option.textContent=`${job.target} · ${new Date(job.createdAt*1000).toLocaleString()} · ${job.id}`; return option;
            }));
            this.manual.restoreSelect.value=sources.some(job=>job.id===selected) ? selected : '';
        }
        this.pinButtons = [];
        this.manual.history.replaceChildren(...data.jobs.map(job=>{
            const item=document.createElement('li');
            item.textContent=`${new Date(job.createdAt*1000).toLocaleString()} · ${job.target} · ${job.kind || "backup"} · ${job.state} · ${job.message || job.id}`;
            if (job.bytes) item.textContent+=` · ${size(job.bytes)}`;
            if (job.sha256) { const hash=document.createElement('small'); hash.textContent=' SHA-256: '+job.sha256; item.append(hash); }
            if (job.kind==='backup' && job.state==='completed') {
                const verification=document.createElement('span'); verification.textContent=job.verifiedAt ? ` · Restore verified ${date(job.verifiedAt)} (${job.verificationSeconds || 0}s)` : ' · Restore not verified'; item.append(verification);
                if (job.sourceId) { const protectedNote=document.createElement('span'); protectedNote.textContent=' · Protected rollback'; item.append(protectedNote); }
                else {
                    const pin=document.createElement('button'); pin.type='button'; pin.className='service-button'; pin.textContent=job.pinned ? 'Unpin archive' : 'Pin archive';
                    pin.addEventListener('click',()=>this.pinArchive(job)); this.pinButtons.push(pin); item.append(pin);
                }
            }
            return item;
        }));
        this.jobPermissions();
    }
    async pinArchive(job) {
        if (!this.allowed || !this.jobData?.canRun || this.jobSending) return;
        const generation=this.generation;
        const id=Array.from(crypto.getRandomValues(new Uint8Array(16)),byte=>byte.toString(16).padStart(2,'0')).join('');
        this.jobSending=true; this.jobPermissions();
        try {
            const data=await this.transport({id,archiveId:job.id,pin:job.pinned ? 0 : 1},'jobs');
            if (generation===this.generation) this.renderJobs(data);
        } catch (error) { if (generation===this.generation) this.manual.result.textContent=error.message; }
        finally { if (generation===this.generation) { this.jobSending=false; this.jobPermissions(); } }
    }
    async loadJobs() {
        if (this.jobLoading || this.jobSending || !this.manual) return;
        const generation=this.generation; this.jobLoading=true; this.jobLastRead=Date.now();
        try {
            const data=await this.transport(undefined,'jobs');
            if (generation===this.generation) this.renderJobs(data);
        } catch(error) {
            if (generation===this.generation && this.manual) { this.jobData=null; this.manual.status.textContent=error.message; this.jobPermissions(); }
        } finally { if (generation===this.generation) this.jobLoading=false; }
    }
    async startBackup(action='backup') {
        const button=action==='restore' ? this.manual?.restore : action==='release' ? this.manual?.release : this.manual?.button;
        if (!button || button.disabled) return;
        const generation=this.generation, target=this.manual.select.value;
        const id=Array.from(crypto.getRandomValues(new Uint8Array(16)),byte=>byte.toString(16).padStart(2,'0')).join('');
        const source=this.jobData.jobs.find(job=>job.id===this.manual.restoreSelect.value);
        const request=action==='restore' ? {id,target:source.target,sourceId:source.id,confirm:this.manual.confirm.value} : action==='release' ? {id,release:'maintenance'} : {id,target};
        this.jobSending=true; this.jobPermissions(); this.manual.result.textContent='Submitting request…';
        try {
            const data=await this.transport(request,'jobs');
            if (generation!==this.generation) return;
            this.renderJobs(data); if(action==='restore') this.manual.confirm.value=''; this.manual.result.textContent=action==='release' ? 'Recovery maintenance ended. Services may be started.' : 'Job requested. Follow its result in history.';
        } catch(error) {
            if (generation===this.generation) this.manual.result.textContent=error.message+' Check job history before trying again. Request '+id;
        } finally { if (generation===this.generation) { this.jobSending=false; this.jobPermissions(); this.loadJobs(); } }
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
        const enabled = field('Automatic backup schedule', '', schedule.enabled, [['0','Disabled'],['1','Enabled']]);
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
                message.textContent = 'Schedule saved. The worker will update the next run time.';
            } catch (error) { if (generation === this.generation) message.textContent = error.message + ' Reload saved schedules before retrying.'; }
            finally { entry.saving = false; if (generation === this.generation) this.permissions(entry); }
        });
    }
};
