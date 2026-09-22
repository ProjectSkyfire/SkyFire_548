/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
"use strict";
window.HubHealthDashboard = class {
    constructor(root) {
        this.root = root; this.history = new Map(); this.cards = new Map(); this.lastSample = 0; this.lastHubUptime = null; this.mapCounters = new Map();
        this.el = (tag, text, parent) => { const element = document.createElement(tag); if (text) element.textContent = text; if (parent) parent.append(element); return element; };
        this.summary = this.el('p', 'Waiting for server health…', root); this.summary.setAttribute('role','status');
        this.health = this.el('div','',root); this.health.className = 'health-grid';
        const label = this.el('label','Server metrics',root); this.select = this.el('select','',label);
        this.select.setAttribute('aria-label','Server for metric graphs'); this.select.addEventListener('change',()=>this.draw());
        this.note = this.el('p','Last 10 minutes in this browser session. Samples every 5 seconds; gaps mean unavailable data.',root);
        this.note.className = 'muted'; this.charts = [];
        const grid = this.el('div','',root); grid.className = 'metric-grid';
        for (const [key,title,unit] of [['players','Players online','players'],['cpu','CPU usage','%'],['update','World update time','ms'],['memoryMiB','Process memory','MiB'],['transfers','Active map transfers','transfers'],['traffic','Map data sent','MiB/s'],['connections','Service connections','connections'],['pendingRequests','Pending character requests','requests'],['latencyMs','Average character request latency','ms'],['failures','Service request failures','failures'],['requests','Chat service requests','requests'],['presencePlayers','Chat player presence','players'],['whisperRelays','Accepted whisper relays','relays']]) {
            const panel = this.el('section','',grid); panel.className = 'metric-panel'; this.el('h2',title,panel);
            const current = this.el('p','Waiting for metrics',panel); current.className = 'metric-value';
            const svg = document.createElementNS('http://www.w3.org/2000/svg','svg');
            svg.setAttribute('viewBox','0 0 360 160'); svg.setAttribute('role','img'); svg.setAttribute('aria-label',title+' over the last 10 minutes'); panel.append(svg);
            this.charts.push({key,title,unit,current,svg,panel});
        }
    }
    reset() {
        this.history.clear(); this.mapCounters.clear(); this.cards.clear(); this.health.replaceChildren(); this.select.replaceChildren();
        this.lastSample = 0; this.lastHubUptime = null; this.latest = null; this.signature = ''; this.summary.textContent = 'Waiting for server health…'; this.draw();
    }
    stale() {
        if (!this.isStale) for (const points of this.history.values()) {
            points.push({at: this.now || Date.now(), players: null, cpu: null, update: null, memoryMiB: null, transfers: null, traffic: null});
            if (points.length > 120) points.shift();
        }
        this.isStale = true; this.summary.textContent = 'Connection lost — health and graphs may be stale.'; this.draw(); }
    update(data, now = Date.now()) {
        if (data.stale) { this.stale(); return; }
        this.isStale = false; this.now = now; this.latest = data;
        if (this.lastHubUptime !== null && data.hubUptime < this.lastHubUptime) { this.history.clear(); this.mapCounters.clear(); }
        this.lastHubUptime = data.hubUptime;
        const services = data.services;
        const running = services.filter(item=>item.state==='running').length;
        this.summary.textContent = `Hub online · uptime ${Math.floor(data.hubUptime/3600)}h ${Math.floor(data.hubUptime%3600/60)}m · ${running}/${services.length} services running`;
        for (const item of services) {
            let card = this.cards.get(item.key);
            if (!card) { card=this.el('article','',this.health); card.className='health-card'; this.el('h2','',card); this.el('p','',card); this.el('small','',card); this.cards.set(item.key,card); }
            card.children[0].textContent=item.name; card.children[1].textContent=item.state;
            card.dataset.state=item.state;
            card.children[2].textContent=(item.mapserver || item.characterserver || item.chatserver) && !item.metricsAvailable ? 'Metrics unavailable' : `Uptime ${Math.floor((item.uptimeSeconds||0)/3600)}h ${Math.floor((item.uptimeSeconds||0)%3600/60)}m`;
            if (item.chatserver && item.metricsAvailable) card.children[2].textContent+=` � Realms ${(item.chatRealms||[]).join(', ')} � ${item.connections} connections � ${item.requests} service requests � ${item.presencePlayers ?? 0} present � ${item.failures} errors`;
            if (item.characterserver && item.metricsAvailable) card.children[2].textContent+=` · DB ${item.databaseReady ? 'ready' : 'unavailable'} · ${item.reads} reads · ${item.writes} writes · ${item.failures} errors`;
            if (item.mapserver && item.metricsAvailable) card.children[2].textContent+=` · Maps ${(item.maps||[]).join(', ')} · ${item.assets} assets · ${item.requests} requests · ${item.failures} errors · ${(item.sentKiB/1024).toFixed(1)} MiB sent`;
        }
        for (const [key,card] of this.cards) if (!services.some(item=>item.key===key)) { card.remove(); this.cards.delete(key); this.history.delete(key); this.mapCounters.delete(key); }
        const worlds=services.filter(item=>item.world || item.mapserver || item.characterserver || item.chatserver);
        const signature=JSON.stringify(worlds.map(item=>[item.key,item.name]));
        if (signature!==this.signature) {
            this.signature=signature; const selected=this.select.value;
            this.select.replaceChildren(...worlds.map(item=>{ const option=this.el('option',item.name); option.value=item.key; return option; }));
            if (worlds.some(item=>item.key===selected)) this.select.value=selected;
        }
        this.select.disabled=!worlds.length;
        if (!this.lastSample || now-this.lastSample>=5000) {
            this.lastSample=now;
            for (const world of worlds) {
                const points=this.history.get(world.key)||[];
                const good=world.state==='running' && world.metricsAvailable;
                const finite=value=>typeof value==='number' && Number.isFinite(value) && value>=0 ? value : null;
                world.traffic=null;
                if (world.mapserver) {
                    const prior=this.mapCounters.get(world.key);
                    if (good && prior && world.uptimeSeconds>=prior.uptime && world.sentKiB>=prior.sent && now>prior.at && now-prior.at<=15000)
                        world.traffic=(world.sentKiB-prior.sent)/1024/((now-prior.at)/1000);
                    if (good) this.mapCounters.set(world.key,{at:now,sent:world.sentKiB,uptime:world.uptimeSeconds});
                    else this.mapCounters.delete(world.key);
                }
                const point={at:now};
                for (const chart of this.charts) point[chart.key]=good?finite(world[chart.key]):null;
                points.push(point);
                while(points.length>120 || (points.length && points[0].at<now-600000)) points.shift();
                this.history.set(world.key,points);
            }
        }
        this.draw();
    }
    draw() {
        const points=this.history.get(this.select.value)||[], now=this.now||Date.now();
        const service=this.latest?.services.find(item=>item.key===this.select.value);
        const ns='http://www.w3.org/2000/svg';
        for (const chart of this.charts) {
            const visible = service?.chatserver ? ['connections','requests','failures','presencePlayers','whisperRelays'] : service?.characterserver ? ['cpu','memoryMiB','connections','pendingRequests','latencyMs','failures'] :
                service?.mapserver ? ['cpu','memoryMiB','transfers','traffic'] : ['players','cpu','update'];
            chart.panel.hidden=!visible.includes(chart.key);
            const usable=service?.state==='running' && service.metricsAvailable && !this.isStale;
            const value=usable ? (chart.key==='traffic' ? points.at(-1)?.traffic : service[chart.key]) : null;
            chart.current.textContent=typeof value==='number' && Number.isFinite(value) && value>=0 ? `${Number(value.toFixed(1))} ${chart.unit}` : this.isStale ? 'Stale — reconnecting' : 'Metrics unavailable';
            const valid=points.filter(point=>typeof point[chart.key]==='number' && point.at>=now-600000);
            const ceiling=Math.max(chart.key==='cpu'?100:1,...valid.map(point=>point[chart.key]));
            chart.svg.replaceChildren();
            const add=(tag,attrs,text)=>{ const node=document.createElementNS(ns,tag); for (const [key,value] of Object.entries(attrs)) node.setAttribute(key,String(value)); if(text!==undefined) node.textContent=text; chart.svg.append(node); return node; };
            for(const fraction of [0,.5,1]) { const y=130-fraction*110; add('line',{x1:38,y1:y,x2:350,y2:y,class:'metric-gridline'}); add('text',{x:2,y:y+4},String(Number((ceiling*fraction).toFixed(1)))); }
            add('text',{x:38,y:153},'−10 min'); add('text',{x:317,y:153},'Now');
            let path='', previous=null;
            for(const point of points) {
                if(typeof point[chart.key]!=='number' || point.at<now-600000) { previous=null; continue; }
                const x=38+312*(point.at-(now-600000))/600000, y=130-110*point[chart.key]/ceiling;
                path+=(previous && point.at-previous.at<=15000?' L':' M')+x.toFixed(2)+' '+y.toFixed(2); previous=point;
            }
            if(path) add('path',{d:path,class:'metric-line'});
            const last=valid.at(-1);
            if(last) add('circle',{cx:38+312*(last.at-(now-600000))/600000,cy:130-110*last[chart.key]/ceiling,r:3,class:'metric-dot'});
        }
    }
};
