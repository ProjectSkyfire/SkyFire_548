/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
'use strict';
// Intercept every request. This test never submits a real backup or restore.
const assert=require('node:assert/strict'), fs=require('node:fs'), path=require('node:path');
const {chromium}=require('playwright');
(async()=>{
 const browser=await chromium.launch({headless:true,...(process.env.HUB_TEST_BROWSER_CHANNEL?{channel:process.env.HUB_TEST_BROWSER_CHANNEL}:{})});
 try {
  const page=await browser.newPage({viewport:{width:1280,height:900}}), errors=[], writes=[];
  page.on('pageerror',error=>errors.push(error.message));
  const web=path.resolve(__dirname,'../../../src/server/hub/web');
  const completed={id:'a'.repeat(32),target:'characters',actor:'admin',kind:'backup',state:'completed',createdAt:1789800000,bytes:2048,sha256:'b'.repeat(64),message:'Dump and checksum verified'};
  const state={available:true,canRun:true,canRestore:true,targets:'auth,characters,world,hub',storageTotal:10000000000,storageFree:4000000000,archiveCount:1,archiveBytes:2048,maintenance:false,recoverySafe:true,jobs:[completed]};
  await page.route('http://hub.test/**',async route=>{
   const url=new URL(route.request().url()), json=value=>route.fulfill({contentType:'application/json',body:JSON.stringify(value)});
   if(url.pathname==='/api/v1/status') return json({username:'admin',csrfToken:'token',canSendWorldCommands:true,canOperateServices:true,accountsEnabled:true,uptimeSeconds:60,components:[]});
   if(url.pathname==='/api/v1/backup/schedules') return json({canEdit:true,schedules:[{target:'characters',enabled:0,mode:'daily',intervalMinutes:60,minuteOfDay:180,weekday:0,revision:1}]});
   if(url.pathname==='/api/v1/backup/jobs') {
    if(route.request().method()==='POST') {
     assert.equal(route.request().headers()['x-hub-csrf'],'token');
     const value=Object.fromEntries(new URLSearchParams(route.request().postData())); writes.push(value);
     if(value.pin!==undefined) state.jobs.find(job=>job.id===value.archiveId).pinned=value.pin==='1';
     else if(value.release) state.maintenance=false;
     else { state.jobs.unshift({id:value.id,target:value.target,kind:value.sourceId?'restore':'backup',state:'queued',createdAt:1789800300,bytes:0,message:''}); if(value.sourceId)state.maintenance=true; }
    }
    return json(state);
   }
   if(url.pathname.startsWith('/api/')) return json({items:[]});
   const file=url.pathname==='/'?'index.html':url.pathname.slice(1);
   if(['index.html','app.css','app.js','status.js','accounts.js','backup.js','navigation.js','metrics.js'].includes(file)) return route.fulfill({contentType:file.endsWith('.js')?'text/javascript':file.endsWith('.css')?'text/css':'text/html',body:fs.readFileSync(path.join(web,file))});
   return route.fulfill({status:404,body:''});
  });
  await page.goto('http://hub.test/'); await page.locator('#backup-tab').click();
  await page.getByLabel('Backup database',{exact:true}).selectOption('characters');
  await page.evaluate(()=>window.HubStatus.stop());
  assert.equal(await page.getByRole('button',{name:'Backup now',exact:true}).isEnabled(),true);
  const time=page.locator('.backup-schedule input[type=time]'); await time.fill('22:35');
  await page.getByRole('button',{name:'Backup now',exact:true}).click();
  await page.waitForFunction(()=>backupSchedules.jobSending===false);
  assert.equal(writes.length,1); assert.equal(writes[0].target,'characters');
  assert.equal(await page.getByRole('button',{name:'Backup now',exact:true}).isEnabled(),false);
  state.jobs[0].state='completed'; await page.evaluate(()=>backupSchedules.loadJobs());
  assert.equal(await time.inputValue(),'22:35');
  await page.getByLabel('Restore recovery point',{exact:true}).selectOption(completed.id);
  assert.equal(await page.getByRole('button',{name:'Restore live database',exact:true}).isEnabled(),false);
  await page.getByLabel('Restore confirmation',{exact:true}).fill('RESTORE characters');
  await page.getByRole('button',{name:'Restore live database',exact:true}).click();
  await page.waitForFunction(()=>backupSchedules.jobSending===false);
  assert.equal(writes.length,2); assert.equal(writes[1].sourceId,completed.id); assert.equal(writes[1].confirm,'RESTORE characters');
  assert.equal(await page.getByRole('button',{name:'End recovery maintenance',exact:true}).isEnabled(),false);
  state.jobs[0].state='completed'; await page.evaluate(()=>backupSchedules.loadJobs());
  await page.getByRole('button',{name:'End recovery maintenance',exact:true}).click();
  await page.waitForFunction(()=>backupSchedules.jobSending===false);
  assert.equal(writes[2].release,'maintenance');
  completed.verifiedAt=1789800050; completed.verificationSeconds=3;
  state.automation=JSON.stringify({retentionDays:30,schedules:[{target:'characters',nextRun:1789800600,status:'Scheduled',backupAt:1789800000,verifiedAt:1789800050}]});
  await page.evaluate(()=>backupSchedules.loadJobs());
  const archiveRow=page.getByLabel('Backup jobs').locator('li').filter({hasText:completed.sha256});
  await archiveRow.getByRole('button',{name:'Pin archive',exact:true}).click();
  await page.waitForFunction(()=>backupSchedules.jobSending===false);
  assert.equal(writes[3].archiveId,completed.id); assert.equal(writes[3].pin,'1');
  assert.equal(await archiveRow.getByRole('button',{name:'Unpin archive',exact:true}).count(),1);
  assert.match(await page.getByLabel('Backup automation').textContent(),/Retention: 30 days/);
  assert.match(await archiveRow.textContent(),/Restore verified/);
  state.available=false; await page.evaluate(()=>backupSchedules.loadJobs());
  assert.equal(await page.getByRole('button',{name:'Backup now',exact:true}).isEnabled(),false);
  assert.match(await page.getByLabel('Backup volume usage').textContent(),/1 archives/);
  state.available=true; state.canRun=false; state.canRestore=false; await page.evaluate(()=>backupSchedules.loadJobs());
  assert.equal(await page.getByRole('button',{name:'Backup now',exact:true}).isEnabled(),false);
  assert.equal(await archiveRow.getByRole('button',{name:'Unpin archive',exact:true}).isEnabled(),false);
  await page.setViewportSize({width:390,height:844});
  assert.equal(await page.evaluate(()=>document.documentElement.scrollWidth<=innerWidth),true,'backup layout must fit mobile');
  assert.deepEqual(errors,[]);
  if(process.env.HUB_TEST_SCREENSHOT)await page.screenshot({path:process.env.HUB_TEST_SCREENSHOT,fullPage:true});
  console.log('PASS manual backup, restore confirmation, maintenance release, permissions, offline handling, stable drafts and mobile storage layout.');
 } finally {await browser.close();}
})().catch(error=>{console.error(error);process.exitCode=1;});
