"use strict";
const assert=require('node:assert/strict');
const path=require('node:path');
const {chromium}=require('playwright');
(async()=>{
 const browser=await chromium.launch({headless:true,...(process.env.HUB_TEST_BROWSER_CHANNEL?{channel:process.env.HUB_TEST_BROWSER_CHANNEL}:{})});
 try {
  const page=await browser.newPage();const errors=[];page.on('pageerror',e=>errors.push(e.message));
  await page.setContent('<div id="health"></div><input id="operator" value="keep my edit">');
  await page.addScriptTag({path:path.resolve(__dirname,'../../../src/server/hub/web/metrics.js')});
  await page.evaluate(()=>{
   window.dashboard=new HubHealthDashboard(document.querySelector('#health'));
   window.fixture={hubUptime:50,services:[{key:'world',name:'World',world:true,state:'running',metricsAvailable:true,players:3,cpu:1,update:2,uptimeSeconds:30},
    {key:'maps-east',name:'Eastern Kingdoms',mapserver:true,state:'running',metricsAvailable:true,cpu:2,memoryMiB:64,transfers:2,sentKiB:1024,requests:10,failures:1,assets:500,maps:[0],uptimeSeconds:25}]};
   dashboard.update(fixture,1000000);
  });
  await page.getByLabel('Server for metric graphs').selectOption('maps-east');
  assert.equal(await page.locator('.metric-panel:visible').count(),4);
  assert.match(await page.locator('.health-card').nth(1).textContent(),/500 assets.*10 requests.*1 errors/);
  await page.locator('#operator').focus();
  await page.evaluate(()=>{fixture.services[1].sentKiB=6144;fixture.services[1].uptimeSeconds=30;dashboard.update(fixture,1005000);});
  assert.match(await page.locator('.metric-panel').filter({hasText:'Map data sent'}).textContent(),/1 MiB\/s/);
  assert.equal(await page.locator('#operator').inputValue(),'keep my edit');
  assert.equal(await page.locator('#operator').evaluate(e=>e===document.activeElement),true);
  await page.evaluate(()=>{fixture.services[1].metricsAvailable=false;fixture.services[1].state='offline';dashboard.update(fixture,1010000);});
  assert.match(await page.locator('.metric-panel').filter({hasText:'Mapserver memory'}).textContent(),/Metrics unavailable/);
  await page.evaluate(()=>dashboard.stale());
  assert.match(await page.locator('.metric-panel').filter({hasText:'Mapserver memory'}).textContent(),/Stale/);
  await page.evaluate(()=>{fixture.services[1].metricsAvailable=true;fixture.services[1].state='running';fixture.services[1].uptimeSeconds=1;fixture.services[1].sentKiB=0;dashboard.update(fixture,1015000);});
  assert.equal(await page.evaluate(()=>dashboard.history.get('maps-east').at(-1).traffic),null);
  await page.getByLabel('Server for metric graphs').selectOption('world');
  assert.equal(await page.locator('.metric-panel:visible').count(),3);
  assert.deepEqual(errors,[]);
  console.log('PASS: map metrics, throughput, stale/offline gaps, restart counters, world graph compatibility, operator input preserved.');
 } finally {await browser.close();}
})().catch(error=>{console.error(error);process.exitCode=1;});
