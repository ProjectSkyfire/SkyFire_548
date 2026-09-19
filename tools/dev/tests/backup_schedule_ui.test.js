/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
const assert = require('node:assert/strict'), fs = require('node:fs'), vm = require('node:vm'), path = require('node:path');
class Element {
    constructor(tag) { this.tag = tag; this.children = []; this.hidden = false; this.events = {}; this.value = ''; }
    append(...items) { for (const item of items) { item.parentElement = this; this.children.push(item); } }
    replaceChildren(...items) { this.children = []; this.append(...items); }
    setAttribute() {}
    addEventListener(name, action) { this.events[name] = action; }
}
async function main() {
    const context = vm.createContext({window:{}, document:{createElement:tag=>new Element(tag)},
        crypto:require('node:crypto').webcrypto, Uint8Array});
    vm.runInContext(fs.readFileSync(path.join(__dirname,'../../../src/server/hub/web/backup.js'),'utf8'),context);
    let reads = 0, writes = [];
    const saved = {canEdit:true, schedules:[{target:'auth',enabled:0,mode:'daily',intervalMinutes:60,minuteOfDay:180,weekday:0,revision:0}]};
    const root = new Element('section');
    const widget = new context.window.HubBackupSchedules(root, async (value, section) => {
        if(section==='jobs') return {available:false,canRun:false,targets:'',jobs:[]};
        if (value) { writes.push(value); saved.schedules[0] = {...value,revision:value.revision+1}; }
        else reads++;
        return saved;
    });
    widget.update(true); await new Promise(resolve=>setImmediate(resolve));
    const form = root.children.find(element=>element.tag==='form');
    const inputs = widget.forms[0].controls;
    const [enabled, mode, interval, time, weekday, button] = inputs;
    time.value = '22:35';
    for (let i=0;i<100;i++) widget.update(true);
    assert.equal(reads,1); assert.equal(time.value,'22:35'); assert.equal(root.children.includes(form),true);
    mode.value = 'weekly'; mode.events.change(); weekday.value = '6'; enabled.value = '1';
    await form.events.submit({preventDefault(){}});
    assert.equal(writes.length,1); assert.equal(writes[0].minuteOfDay,1355); assert.equal(writes[0].weekday,6);
    assert.equal(writes[0].mode,'weekly'); assert.equal(writes[0].revision,0);
    assert.equal(time.value,'22:35');
    mode.value = 'interval'; mode.events.change(); interval.value = '45';
    assert.equal(time.disabled,true); assert.equal(interval.disabled,false);
    await form.events.submit({preventDefault(){}});
    assert.equal(writes[1].revision,1); assert.equal(writes[1].intervalMinutes,45); assert.equal(writes[1].minuteOfDay,0);
    widget.update(false); assert.equal(button.disabled,true);
    await form.events.submit({preventDefault(){}}); assert.equal(writes.length,2);
    assert.match(form.children.at(-1).textContent,/no scheduled backups are running/);
    widget.reset(); assert.equal(root.children.length,0);
    console.log('PASS schedule drafts survive 100 status updates; UTC/interval saves, revision advancement, permission revocation and logout reset.');
}
main().catch(error=>{console.error(error);process.exitCode=1;});
