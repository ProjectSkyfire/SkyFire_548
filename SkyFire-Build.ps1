#Requires -Version 5.1
<#
.SYNOPSIS
    Simple SkyFire Windows helper: git pull, CMake configure/build/install, and server config management.

.DESCRIPTION
    Based on the Project SkyFire Windows wiki install steps.
    Copy skyfire-build.config.ps1.example to skyfire-build.config.ps1 to customize paths and CMake options.
    Log file: skyfire-build.log (next to this script)

.LINK
    https://wiki.projectskyfire.org/index.php?title=Installation_(Windows_5xx)
#>
[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [ValidateSet('menu', 'update', 'configure', 'build', 'install', 'build-install', 'backup-configs', 'install-configs', 'show-config')]
    [string] $Action = 'menu'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Script:RepoRoot = $PSScriptRoot
$Script:LogFile = Join-Path $Script:RepoRoot 'skyfire-build.log'
$Script:LocalConfigPath = Join-Path $Script:RepoRoot 'skyfire-build.config.ps1'
$Script:LastConfigWriteTime = $null
$Script:LoadedConfigFingerprint = $null

function Write-SkyFireLog {
    param(
        [string] $Message,
        [ValidateSet('INFO', 'WARN', 'ERROR')]
        [string] $Level = 'INFO'
    )

    $line = '{0:yyyy-MM-dd HH:mm:ss} [{1}] {2}' -f (Get-Date), $Level, $Message
    Add-Content -LiteralPath $Script:LogFile -Value $line -Encoding UTF8
}

function Write-SkyFireError {
    param(
        [string] $Message,
        [System.Management.Automation.ErrorRecord] $ErrorRecord
    )

    Write-Host ''
    Write-Host $Message -ForegroundColor Red
    if ($ErrorRecord) {
        Write-Host $ErrorRecord.Exception.Message -ForegroundColor Red
    }
    Write-Host "Details written to $Script:LogFile" -ForegroundColor Yellow
    Write-Host ''

    Write-SkyFireLog -Message $Message -Level ERROR
    if ($ErrorRecord) {
        Write-SkyFireLog -Message $ErrorRecord.Exception.Message -Level ERROR
        if ($ErrorRecord.ScriptStackTrace) {
            Write-SkyFireLog -Message $ErrorRecord.ScriptStackTrace -Level ERROR
        }
    }
}

function Wait-ForUserIfNeeded {
    param([switch] $Force)

    if ($Force -or ($Action -eq 'menu' -and $Host.Name -eq 'ConsoleHost')) {
        Read-Host 'Press Enter to continue'
    }
}

function Get-DefaultSkyFireBuildConfig {
    @{
        BoostRoot         = 'C:\local\boost_1_91_0'
        OpenSslRootDir    = 'C:\Program Files\OpenSSL-Win64'
        OpenSslModules    = 'C:\Program Files\OpenSSL-Win64\bin'
        MySqlRoot         = 'C:\tools\mysql\current'
        SourceDir         = $Script:RepoRoot
        BuildDir          = Join-Path $Script:RepoRoot 'build'
        InstallPrefix     = 'C:\SkyFire_Files\Server'
        CMakeGenerator    = 'Visual Studio 17 2022'
        CMakeArch         = 'x64'
        BuildConfig       = 'Release'
        CMakeOptions      = @{
            TOOLS               = 'ON'
            USE_COREPCH         = 'ON'
            USE_SCRIPTPCH       = 'ON'
            SKYFIRE_BUILD_TESTS = 'OFF'
            AUTH_SERVER         = 'ON'
            SCRIPTS             = 'ON'
        }
        BuildParallelJobs = 0
    }
}

function Resolve-SkyFireSourceDir {
    param(
        [hashtable] $Config
    )

    $candidates = @(
        $Config.SourceDir
        $Script:RepoRoot
    ) | Where-Object { -not [string]::IsNullOrWhiteSpace($_) } | Select-Object -Unique

    foreach ($candidate in $candidates) {
        $gitPath = Join-Path $candidate '.git'
        if (Test-Path -LiteralPath $gitPath) {
            if ($candidate -ne $Config.SourceDir) {
                Write-SkyFireLog -Message "SourceDir '$($Config.SourceDir)' is not a git repo; using '$candidate'." -Level WARN
                Write-Host "Using git repo: $candidate" -ForegroundColor Yellow
            }
            return $candidate
        }
    }

    throw "No git repository found. Checked: $($candidates -join ', ')"
}

function Get-SkyFireBuildConfig {
    $config = Get-DefaultSkyFireBuildConfig
    $localConfigPath = Join-Path $Script:RepoRoot 'skyfire-build.config.ps1'

    if (Test-Path -LiteralPath $localConfigPath) {
        $loaded = & $localConfigPath
        if ($null -ne $loaded) {
            foreach ($key in $loaded.Keys) {
                $config[$key] = $loaded[$key]
            }
        }
    }

    if ([string]::IsNullOrWhiteSpace([string]$config.SourceDir)) {
        $config.SourceDir = $Script:RepoRoot
    }

    if ([string]::IsNullOrWhiteSpace([string]$config.BuildDir)) {
        $config.BuildDir = Join-Path $config.SourceDir 'build'
    }

    if ($null -eq $config.CMakeOptions) {
        $config.CMakeOptions = @{}
    }

    $config.SourceDir = Resolve-SkyFireSourceDir -Config $config

    return $config
}

function Get-SkyFireConfigFingerprint {
    param([hashtable] $Config)

    $cmakeOptions = ($Config.CMakeOptions.GetEnumerator() | Sort-Object Name | ForEach-Object {
        "{0}={1}" -f $_.Key, $_.Value
    }) -join ';'

    return @(
        $Config.BoostRoot
        $Config.OpenSslRootDir
        $Config.OpenSslModules
        $Config.MySqlRoot
        $Config.SourceDir
        $Config.BuildDir
        $Config.InstallPrefix
        $Config.CMakeGenerator
        $Config.CMakeArch
        $Config.BuildConfig
        $Config.BuildParallelJobs
        $cmakeOptions
    ) -join '|'
}

function Update-SkyFireRuntimeConfig {
    param(
        [ref] $Config,
        [switch] $AnnounceChanges
    )

    $configChanged = $false
    if (Test-Path -LiteralPath $Script:LocalConfigPath) {
        $writeTime = (Get-Item -LiteralPath $Script:LocalConfigPath).LastWriteTimeUtc
        if ($null -ne $Script:LastConfigWriteTime -and $writeTime -ne $Script:LastConfigWriteTime) {
            $configChanged = $true
        }
        $Script:LastConfigWriteTime = $writeTime
    }

    $newConfig = Get-SkyFireBuildConfig
    $newFingerprint = Get-SkyFireConfigFingerprint -Config $newConfig
    if ($null -ne $Script:LoadedConfigFingerprint -and $newFingerprint -ne $Script:LoadedConfigFingerprint) {
        $configChanged = $true
    }

    $Config.Value = $newConfig
    $Script:LoadedConfigFingerprint = $newFingerprint

    if ($configChanged -and $AnnounceChanges) {
        Write-Host 'Reloaded skyfire-build.config.ps1' -ForegroundColor Green
        Write-SkyFireLog -Message 'Reloaded skyfire-build.config.ps1'
    }

    return $configChanged
}

function Write-SkyFireBuildConfig {
    param([hashtable] $Config)

    Write-Host ''
    Write-Host 'Current SkyFire build settings' -ForegroundColor Cyan
    Write-Host "  SourceDir      : $($Config.SourceDir)"
    Write-Host "  BuildDir       : $($Config.BuildDir)"
    Write-Host "  InstallPrefix  : $($Config.InstallPrefix)"
    Write-Host "  Generator      : $($Config.CMakeGenerator) ($($Config.CMakeArch))"
    Write-Host "  BuildConfig    : $($Config.BuildConfig)"
    Write-Host "  BOOST_ROOT     : $($Config.BoostRoot)"
    Write-Host "  OPENSSL_ROOT   : $($Config.OpenSslRootDir)"
    Write-Host "  MYSQL_ROOT     : $($Config.MySqlRoot)"
    Write-Host "  Log file       : $Script:LogFile"
    Write-Host '  CMake options  :'
    foreach ($option in ($Config.CMakeOptions.Keys | Sort-Object)) {
        Write-Host "    -D$option=$($Config.CMakeOptions[$option])"
    }
    Write-Host ''
}

function Set-SkyFireBuildEnvironment {
    param([hashtable] $Config)

    $env:BOOST_ROOT = $Config.BoostRoot
    $env:OPENSSL_ROOT_DIR = $Config.OpenSslRootDir
    $env:OPENSSL_MODULES = $Config.OpenSslModules
    $env:MYSQL_ROOT = $Config.MySqlRoot
}

function Invoke-SkyFireNativeCommand {
    param(
        [Parameter(Mandatory)]
        [scriptblock] $Command,
        [object[]] $ArgumentList = @(),
        [switch] $EchoOutput
    )

    # Native tools (git, cmake) write progress to stderr. With $ErrorActionPreference = 'Stop',
    # PowerShell treats that as a terminating error unless we relax it for the call.
    $savedEap = $ErrorActionPreference
    $savedNative = $null
    if (Get-Variable -Name PSNativeCommandUseErrorActionPreference -ErrorAction SilentlyContinue) {
        $savedNative = $PSNativeCommandUseErrorActionPreference
        $PSNativeCommandUseErrorActionPreference = $false
    }

    $ErrorActionPreference = 'Continue'
    try {
        $output = & $Command @ArgumentList 2>&1
    }
    finally {
        $ErrorActionPreference = $savedEap
        if ($null -ne $savedNative) {
            $PSNativeCommandUseErrorActionPreference = $savedNative
        }
    }

    foreach ($line in @($output)) {
        if ($null -eq $line) {
            continue
        }

        $text = if ($line -is [System.Management.Automation.ErrorRecord]) {
            $line.ToString()
        }
        else {
            [string]$line
        }

        if ($EchoOutput) {
            Write-Host $text
            Write-SkyFireLog -Message $text
        }
    }

    return $LASTEXITCODE
}

function Invoke-SkyFireGitUpdate {
    param([hashtable] $Config)

    $sourceDir = $Config.SourceDir
    Write-SkyFireLog -Message "Starting git pull in $sourceDir"

    Write-Host "Running git pull in $sourceDir ..." -ForegroundColor Cyan

    $savedGitStderrRedirect = $env:GIT_REDIRECT_STDERR
    $env:GIT_REDIRECT_STDERR = '2>&1'
    try {
        $exitCode = Invoke-SkyFireNativeCommand -EchoOutput -ArgumentList $sourceDir -Command {
            param([string] $RepoPath)
            & git -C $RepoPath pull
        }
    }
    finally {
        if ($null -eq $savedGitStderrRedirect) {
            Remove-Item Env:\GIT_REDIRECT_STDERR -ErrorAction SilentlyContinue
        }
        else {
            $env:GIT_REDIRECT_STDERR = $savedGitStderrRedirect
        }
    }
    if ($exitCode -ne 0) {
        $hint = 'git pull failed. Commit or stash local changes, or run "git status" in the repo folder.'
        Write-SkyFireLog -Message "git pull failed with exit code $exitCode. $hint" -Level ERROR
        throw "git pull failed with exit code $exitCode. $hint"
    }

    Write-Host 'Repository updated.' -ForegroundColor Green
    Write-SkyFireLog -Message 'git pull completed successfully'
}

function Invoke-SkyFireConfigure {
    param([hashtable] $Config)

    Set-SkyFireBuildEnvironment -Config $Config
    Write-SkyFireLog -Message 'Starting CMake configure'

    $cmakeArgs = @(
        '-S', $Config.SourceDir
        '-B', $Config.BuildDir
        '-G', $Config.CMakeGenerator
        '-A', $Config.CMakeArch
        "-DCMAKE_INSTALL_PREFIX=$($Config.InstallPrefix)"
        "-DBOOST_ROOT=$($Config.BoostRoot)"
        "-DOPENSSL_ROOT_DIR=$($Config.OpenSslRootDir)"
    )

    foreach ($optionName in ($Config.CMakeOptions.Keys | Sort-Object)) {
        $cmakeArgs += "-D$optionName=$($Config.CMakeOptions[$optionName])"
    }

    $commandLine = "cmake $($cmakeArgs -join ' ')"
    Write-Host 'Running CMake configure...' -ForegroundColor Cyan
    Write-Host "  $commandLine"
    Write-SkyFireLog -Message $commandLine

    & cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configure failed with exit code $LASTEXITCODE"
    }

    Write-Host 'CMake configure finished.' -ForegroundColor Green
    Write-SkyFireLog -Message 'CMake configure completed successfully'
}

function Invoke-SkyFireBuild {
    param([hashtable] $Config)

    Write-SkyFireLog -Message 'Starting CMake build'

    $buildArgs = @(
        '--build', $Config.BuildDir
        '--config', $Config.BuildConfig
    )

    if ([int]$Config.BuildParallelJobs -gt 0) {
        $buildArgs += @('--parallel', [string]$Config.BuildParallelJobs)
    }
    else {
        $buildArgs += '--parallel'
    }

    $commandLine = "cmake $($buildArgs -join ' ')"
    Write-Host 'Running CMake build...' -ForegroundColor Cyan
    Write-Host "  $commandLine"
    Write-SkyFireLog -Message $commandLine

    & cmake @buildArgs
    if ($LASTEXITCODE -ne 0) {
        throw "CMake build failed with exit code $LASTEXITCODE"
    }

    Write-Host 'Build finished.' -ForegroundColor Green
    Write-SkyFireLog -Message 'CMake build completed successfully'
}

function Invoke-SkyFireInstall {
    param([hashtable] $Config)

    Write-SkyFireLog -Message 'Starting CMake install'

    $installArgs = @(
        '--install', $Config.BuildDir
        '--config', $Config.BuildConfig
    )

    $commandLine = "cmake $($installArgs -join ' ')"
    Write-Host 'Running CMake install...' -ForegroundColor Cyan
    Write-Host "  $commandLine"
    Write-SkyFireLog -Message $commandLine

    & cmake @installArgs
    if ($LASTEXITCODE -ne 0) {
        throw "CMake install failed with exit code $LASTEXITCODE"
    }

    Write-Host "Installed to $($Config.InstallPrefix)" -ForegroundColor Green
    Write-SkyFireLog -Message "CMake install completed successfully to $($Config.InstallPrefix)"
}

function Backup-SkyFireServerConfigs {
    param([hashtable] $Config)

    $installDir = $Config.InstallPrefix
    if (-not (Test-Path -LiteralPath $installDir)) {
        throw "Install directory not found: $installDir"
    }

    foreach ($configName in @('authserver.conf', 'worldserver.conf')) {
        $configPath = Join-Path $installDir $configName
        if (Test-Path -LiteralPath $configPath) {
            $backupPath = "$configPath.back"
            Copy-Item -LiteralPath $configPath -Destination $backupPath -Force
            Write-Host "Backed up $configName -> $(Split-Path -Leaf $backupPath)" -ForegroundColor Green
            Write-SkyFireLog -Message "Backed up $configPath to $backupPath"
        }
        else {
            Write-Host "Skipped $configName (file not found)" -ForegroundColor Yellow
            Write-SkyFireLog -Message "Skipped backup for missing $configPath" -Level WARN
        }
    }
}

function Install-SkyFireDistConfigs {
    param(
        [hashtable] $Config,
        [switch] $OverwriteExisting
    )

    $installDir = $Config.InstallPrefix
    if (-not (Test-Path -LiteralPath $installDir)) {
        throw "Install directory not found: $installDir"
    }

    foreach ($baseName in @('authserver', 'worldserver')) {
        $distPath = Join-Path $installDir "$baseName.conf.dist"
        $confPath = Join-Path $installDir "$baseName.conf"

        if (-not (Test-Path -LiteralPath $distPath)) {
            Write-Host "Skipped $baseName.conf (missing $baseName.conf.dist)" -ForegroundColor Yellow
            continue
        }

        if ((Test-Path -LiteralPath $confPath) -and -not $OverwriteExisting) {
            $answer = Read-Host "$baseName.conf already exists. Overwrite? [y/N]"
            if ($answer -notmatch '^(y|yes)$') {
                Write-Host "Skipped $baseName.conf" -ForegroundColor Yellow
                continue
            }
        }

        Copy-Item -LiteralPath $distPath -Destination $confPath -Force
        Write-Host "Created $baseName.conf from $baseName.conf.dist" -ForegroundColor Green
        Write-SkyFireLog -Message "Installed $confPath from $distPath"
    }
}

function Open-SkyFireLocalConfig {
    $examplePath = Join-Path $Script:RepoRoot 'skyfire-build.config.ps1.example'
    $localPath = Join-Path $Script:RepoRoot 'skyfire-build.config.ps1'

    if (-not (Test-Path -LiteralPath $localPath)) {
        if (-not (Test-Path -LiteralPath $examplePath)) {
            throw "Missing config template: $examplePath"
        }

        Copy-Item -LiteralPath $examplePath -Destination $localPath
        Write-Host "Created $localPath from example." -ForegroundColor Green
    }

    Start-Process notepad.exe -ArgumentList $localPath -Wait
    Write-Host 'Config editor closed. Settings reload automatically on the next menu action.'
}

function Invoke-SkyFireMenuAction {
    param(
        [ref] $Config,
        [scriptblock] $ActionBlock,
        [string] $ActionName
    )

    try {
        Update-SkyFireRuntimeConfig -Config $Config -AnnounceChanges | Out-Null
        Write-SkyFireLog -Message "Menu action started: $ActionName"
        & $ActionBlock
        Write-SkyFireLog -Message "Menu action finished: $ActionName"
    }
    catch {
        Write-SkyFireError -Message "$ActionName failed." -ErrorRecord $_
        Wait-ForUserIfNeeded
    }
}

function Show-SkyFireBuildMenu {
    $Config = Get-SkyFireBuildConfig
    $null = Update-SkyFireRuntimeConfig -Config ([ref]$Config)

    Write-SkyFireLog -Message 'SkyFire-Build menu opened'

    while ($true) {
        Update-SkyFireRuntimeConfig -Config ([ref]$Config) -AnnounceChanges | Out-Null

        Write-Host ''
        Write-Host '========================================' -ForegroundColor Cyan
        Write-Host ' SkyFire Windows Build Helper' -ForegroundColor Cyan
        Write-Host '========================================' -ForegroundColor Cyan
        Write-Host " Repo: $($Config.SourceDir)"
        Write-Host " Log:  $Script:LogFile"
        Write-Host '----------------------------------------'
        Write-Host ' 1) Update            (git pull)'
        Write-Host ' 2) Configure CMake'
        Write-Host ' 3) Build'
        Write-Host ' 4) Install'
        Write-Host ' 5) Build + Install   (configure, build, install)'
        Write-Host ' 6) Backup configs    (.conf -> .conf.back)'
        Write-Host ' 7) Install configs   (.conf.dist -> .conf)'
        Write-Host ' 8) Show settings'
        Write-Host ' 9) Open local config (skyfire-build.config.ps1)'
        Write-Host ' 0) Exit'
        Write-Host ''

        $choice = Read-Host 'Select an option'
        Update-SkyFireRuntimeConfig -Config ([ref]$Config) -AnnounceChanges | Out-Null
        switch ($choice) {
            '1' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Update' -ActionBlock { Invoke-SkyFireGitUpdate -Config $Config } }
            '2' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Configure' -ActionBlock { Invoke-SkyFireConfigure -Config $Config } }
            '3' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Build' -ActionBlock { Invoke-SkyFireBuild -Config $Config } }
            '4' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Install' -ActionBlock { Invoke-SkyFireInstall -Config $Config } }
            '5' {
                Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Build + Install' -ActionBlock {
                    Invoke-SkyFireConfigure -Config $Config
                    Invoke-SkyFireBuild -Config $Config
                    Invoke-SkyFireInstall -Config $Config
                }
            }
            '6' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Backup configs' -ActionBlock { Backup-SkyFireServerConfigs -Config $Config } }
            '7' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionBlock { Install-SkyFireDistConfigs -Config $Config } -ActionName 'Install configs' }
            '8' { Write-SkyFireBuildConfig -Config $Config }
            '9' { Invoke-SkyFireMenuAction -Config ([ref]$Config) -ActionName 'Open config' -ActionBlock { Open-SkyFireLocalConfig } }
            '0' { return }
            default { Write-Host 'Invalid choice.' -ForegroundColor Yellow }
        }
    }
}

function Invoke-SkyFireAction {
    param(
        [hashtable] $Config,
        [string] $ActionName
    )

    switch ($ActionName) {
        'menu' { Show-SkyFireBuildMenu }
        'update' { Invoke-SkyFireGitUpdate -Config $Config }
        'configure' { Invoke-SkyFireConfigure -Config $Config }
        'build' { Invoke-SkyFireBuild -Config $Config }
        'install' { Invoke-SkyFireInstall -Config $Config }
        'build-install' {
            Invoke-SkyFireConfigure -Config $Config
            Invoke-SkyFireBuild -Config $Config
            Invoke-SkyFireInstall -Config $Config
        }
        'backup-configs' { Backup-SkyFireServerConfigs -Config $Config }
        'install-configs' { Install-SkyFireDistConfigs -Config $Config }
        'show-config' { Write-SkyFireBuildConfig -Config $Config }
        default { throw "Unknown action: $ActionName" }
    }
}

try {
    Write-SkyFireLog -Message "SkyFire-Build started (action=$Action)"
    $config = Get-SkyFireBuildConfig
    Invoke-SkyFireAction -Config $config -ActionName $Action
    Write-SkyFireLog -Message "SkyFire-Build finished (action=$Action)"
}
catch {
    Write-SkyFireError -Message 'SkyFire-Build failed.' -ErrorRecord $_
    Wait-ForUserIfNeeded -Force
    exit 1
}
