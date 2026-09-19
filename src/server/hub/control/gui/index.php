<?php
/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
// PHP renders only the interface shell. All authenticated data comes from the Python API.
$brand = 'Project SkyFire';
?>
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="color-scheme" content="dark">
    <title><?= htmlspecialchars($brand, ENT_QUOTES, 'UTF-8') ?> · Cluster control</title>
    <link rel="stylesheet" href="/control.css">
    <script src="/navigation.js" defer></script>
    <script src="/metrics.js" defer></script>
    <script src="/backup.js" defer></script>
    <script src="/control.js" defer></script>
</head>
<body>
    <header><a href="/" class="brand"><?= htmlspecialchars($brand, ENT_QUOTES, 'UTF-8') ?></a><span>Cluster control</span><nav id="control-nav" aria-label="Hub navigation" hidden><button data-page="health" type="button">Server health</button><button id="control-console-tab" data-page="console" type="button">Console</button><button data-page="backup" type="button">Backup</button></nav><button id="logout" hidden>Sign out</button></header>
    <main>
        <section id="login-panel" class="login panel">
            <p class="eyebrow">Remote operations</p><h1>Sign in to your hub</h1>
            <p class="muted">Use a hub account with remote access enabled.</p>
            <form id="login-form">
                <label for="username">Username</label><input id="username" name="username" autocomplete="username" maxlength="64" required>
                <label for="password">Password</label><input id="password" name="password" type="password" autocomplete="current-password" maxlength="1024" required>
                <button type="submit" class="primary">Sign in</button>
            </form>
            <p id="login-error" role="alert"></p>
        </section>
        <div id="workspace" hidden>
            <aside class="sidebar">
                <p class="eyebrow">Server status</p><h2 id="hub-name">Hub</h2>
                <p id="connection" role="status">Connecting…</p><p id="identity" class="muted"></p>
                <div id="services"></div>
                <p class="muted note">Status updates independently. Forms stay in place while you work.</p>
            </aside>
            <div class="content">
                <div data-page-view="health">
                <div class="heading"><div><p class="eyebrow">Live directory</p><h1>Server health</h1></div><span id="updated" class="muted"></span></div>
                <div id="control-health-dashboard"></div>
                <section class="panel"><h2>World realms</h2><div id="realms" class="table-wrap"></div></section>
                <section class="panel"><h2>Registered nodes</h2><div id="nodes" class="table-wrap"></div></section>
                <section class="panel"><h2>Authentication routing</h2><div id="routes" class="table-wrap"></div></section>
                </div>
                <div data-page-view="console" hidden><h1>Console</h1>
                <section id="operations" class="panel" hidden>
                    <p class="eyebrow">Controlled operations</p><h2>Send a command</h2>
                    <form id="command-form">
                        <div class="fields"><div><label for="action">Action</label><select id="action" name="action">
                            <option value="cluster.drain">Drain node</option><option value="cluster.disable">Disable node routing</option><option value="cluster.enable">Enable node routing</option>
                            <option value="service.start">Start managed service</option><option value="service.stop">Stop managed service</option>
                            <option value="world.restart">Restart world after countdown</option><option value="world.shutdown">Shut down world after countdown</option>
                            <option value="world.cancel-restart">Cancel world restart</option><option value="world.cancel-shutdown">Cancel world shutdown</option>
                        </select></div><div><label for="target">Node / service</label><select id="target" name="target" required></select></div>
                        <div id="countdown-field" hidden><label for="seconds">Countdown in seconds</label><input id="seconds" type="number" min="0" max="86400" step="1" value="300"></div></div>
                        <p id="command-help" class="muted">Draining stops new routes. Existing sessions remain connected.</p>
                        <button id="send" class="primary" type="submit">Send command</button>
                    </form>
                    <p id="command-result" role="status" aria-live="polite"></p>
                </section>
                <section class="panel"><h2>Recent operations</h2><ol id="events" class="event-list"></ol><p id="no-events" class="muted">No commands submitted in this session.</p></section>
                </div>
                <div data-page-view="backup" hidden><h1>Backup</h1><section id="backup-schedules" class="panel"></section></div>
            </div>
        </div>
    </main>
</body>
</html>
