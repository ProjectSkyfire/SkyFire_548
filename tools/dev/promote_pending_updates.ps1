<#
.SYNOPSIS
Promotes pending SQL updates into the normal update stream.

.DESCRIPTION
Moves SQL files from sql/pending_updates/<database> to sql/updates/<database>
and assigns each promoted file the next available official update name:
YYYY_MM_DD_<database>_NN.sql.

The script runs in dry-run mode by default. Use -Apply to move files.

.EXAMPLE
powershell -NoProfile -ExecutionPolicy Bypass -File tools/dev/promote_pending_updates.ps1

.EXAMPLE
powershell -NoProfile -ExecutionPolicy Bypass -File tools/dev/promote_pending_updates.ps1 -Database world -Apply

.EXAMPLE
powershell -NoProfile -ExecutionPolicy Bypass -File tools/dev/promote_pending_updates.ps1 -InputPath sql/pending_updates/world/areatrigger_tavern_missing.sql -Apply
#>

[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [string] $Root = '',

    [string] $PromotionDate = '',

    [ValidateSet('auth', 'characters', 'world')]
    [string[]] $Database,

    [string[]] $InputPath,

    [switch] $Apply
)

$ErrorActionPreference = 'Stop'

$ValidDatabases = @('auth', 'characters', 'world')
$UpdateNamePattern = '^(?<date>\d{4}_\d{2}_\d{2})_(?<database>auth|characters|world)_(?<sequence>\d{2})(?<suffix>.*)\.sql$'

function Resolve-PromotionDate {
    param([string] $RequestedDate)

    if ([string]::IsNullOrWhiteSpace($RequestedDate)) {
        return (Get-Date).ToString('yyyy_MM_dd')
    }

    if ($RequestedDate -notmatch '^\d{4}_\d{2}_\d{2}$') {
        throw "PromotionDate '$RequestedDate' must use YYYY_MM_DD."
    }

    return $RequestedDate
}

function Resolve-RepoRoot {
    param([string] $RequestedRoot)

    if ($RequestedRoot) {
        return (Resolve-Path -LiteralPath $RequestedRoot).ProviderPath
    }

    $gitRoot = & git rev-parse --show-toplevel 2>$null
    if ($LASTEXITCODE -eq 0 -and $gitRoot) {
        return (Resolve-Path -LiteralPath $gitRoot.Trim()).ProviderPath
    }

    return (Resolve-Path -LiteralPath '.').ProviderPath
}

function Convert-ToRepoPath {
    param(
        [string] $Path,
        [string] $RootPath
    )

    $fullPath = [System.IO.Path]::GetFullPath($Path)
    $fullRoot = [System.IO.Path]::GetFullPath($RootPath)

    if (-not $fullRoot.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
        $fullRoot += [System.IO.Path]::DirectorySeparatorChar
    }

    if ($fullPath.StartsWith($fullRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
        return $fullPath.Substring($fullRoot.Length).Replace('\', '/')
    }

    return $fullPath.Replace('\', '/')
}

function Test-IsUnderPath {
    param(
        [string] $Path,
        [string] $ParentPath
    )

    $fullPath = [System.IO.Path]::GetFullPath($Path)
    $fullParent = [System.IO.Path]::GetFullPath($ParentPath)

    if (-not $fullParent.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
        $fullParent += [System.IO.Path]::DirectorySeparatorChar
    }

    return $fullPath.StartsWith($fullParent, [System.StringComparison]::OrdinalIgnoreCase)
}

function Get-SelectedDatabases {
    param([string[]] $RequestedDatabases)

    if ($RequestedDatabases -and $RequestedDatabases.Count -gt 0) {
        return $RequestedDatabases
    }

    return $ValidDatabases
}

function Get-PendingSqlFiles {
    param(
        [string] $RootPath,
        [string[]] $RequestedDatabases,
        [string[]] $RequestedPaths
    )

    $files = @()

    if ($RequestedPaths -and $RequestedPaths.Count -gt 0) {
        foreach ($requestedPath in $RequestedPaths) {
            $candidate = $requestedPath
            if (-not [System.IO.Path]::IsPathRooted($candidate)) {
                $candidate = Join-Path $RootPath $candidate
            }

            $resolved = Resolve-Path -LiteralPath $candidate
            foreach ($item in $resolved) {
                if (Test-Path -LiteralPath $item.ProviderPath -PathType Container) {
                    $files += Get-ChildItem -LiteralPath $item.ProviderPath -Filter '*.sql' -File | Sort-Object FullName
                }
                else {
                    $files += Get-Item -LiteralPath $item.ProviderPath
                }
            }
        }
    }
    else {
        foreach ($databaseName in (Get-SelectedDatabases -RequestedDatabases $RequestedDatabases)) {
            $pendingDir = Join-Path $RootPath "sql\pending_updates\$databaseName"
            if (Test-Path -LiteralPath $pendingDir -PathType Container) {
                $files += Get-ChildItem -LiteralPath $pendingDir -Filter '*.sql' -File | Sort-Object FullName
            }
        }
    }

    return $files | Sort-Object FullName -Unique
}

function Get-PendingUpdateInfo {
    param(
        [System.IO.FileInfo] $File,
        [string] $RootPath
    )

    if ($File.Extension -ne '.sql') {
        throw "Pending update '$($File.FullName)' is not a .sql file."
    }

    $pendingRoot = Join-Path $RootPath 'sql\pending_updates'
    if (-not (Test-IsUnderPath -Path $File.FullName -ParentPath $pendingRoot)) {
        throw "Refusing '$($File.FullName)' because it is not under sql/pending_updates."
    }

    $relativePath = Convert-ToRepoPath -Path $File.FullName -RootPath $RootPath
    $relativeParts = $relativePath.Split('/')
    if ($relativeParts.Count -ne 4 -or $relativeParts[0] -ne 'sql' -or $relativeParts[1] -ne 'pending_updates') {
        throw "Refusing '$relativePath' because pending updates must be directly under sql/pending_updates/<database>."
    }

    $databaseName = $relativeParts[2]
    if ($ValidDatabases -notcontains $databaseName) {
        throw "Refusing '$relativePath' because '$databaseName' is not a known database folder."
    }

    return [PSCustomObject]@{
        Source = $File
        Database = $databaseName
    }
}

function Get-MaxOfficialSequence {
    param(
        [string] $RootPath,
        [string] $DatabaseName,
        [string] $DateText
    )

    $updatesDir = Join-Path $RootPath "sql\updates\$DatabaseName"
    if (-not (Test-Path -LiteralPath $updatesDir -PathType Container)) {
        return -1
    }

    $maxSequence = -1
    foreach ($file in (Get-ChildItem -LiteralPath $updatesDir -Filter '*.sql' -File)) {
        $match = [regex]::Match($file.Name, $UpdateNamePattern)
        if (-not $match.Success) {
            continue
        }

        if ($match.Groups['database'].Value -ne $DatabaseName -or $match.Groups['date'].Value -ne $DateText) {
            continue
        }

        $sequence = [int] $match.Groups['sequence'].Value
        if ($sequence -gt $maxSequence) {
            $maxSequence = $sequence
        }
    }

    return $maxSequence
}

function New-PromotionPlan {
    param(
        [string] $RootPath,
        [System.IO.FileInfo[]] $Files,
        [string] $DateText
    )

    $infos = @()
    foreach ($file in $Files) {
        $infos += Get-PendingUpdateInfo -File $file -RootPath $RootPath
    }

    $actions = @()
    $groups = $infos | Group-Object Database

    foreach ($group in $groups) {
        $first = $group.Group | Select-Object -First 1
        $nextSequence = (Get-MaxOfficialSequence -RootPath $RootPath -DatabaseName $first.Database -DateText $DateText) + 1
        $updatesDir = Join-Path $RootPath "sql\updates\$($first.Database)"

        foreach ($info in ($group.Group | Sort-Object { $_.Source.Name })) {
            $targetName = '{0}_{1}_{2:D2}.sql' -f $DateText, $info.Database, $nextSequence
            $targetPath = Join-Path $updatesDir $targetName

            if (Test-Path -LiteralPath $targetPath) {
                throw "Target update already exists: $(Convert-ToRepoPath -Path $targetPath -RootPath $RootPath)"
            }

            $actions += [PSCustomObject]@{
                Source = $info.Source.FullName
                Target = $targetPath
                Database = $info.Database
            }

            $nextSequence++
        }
    }

    return $actions | Sort-Object Database, Source
}

function Invoke-PromotionPlan {
    param(
        [string] $RootPath,
        [object[]] $Actions,
        [switch] $ApplyChanges
    )

    foreach ($action in $Actions) {
        $sourceRelative = Convert-ToRepoPath -Path $action.Source -RootPath $RootPath
        $targetRelative = Convert-ToRepoPath -Path $action.Target -RootPath $RootPath

        if (-not $ApplyChanges) {
            Write-Host "DRY-RUN: $sourceRelative -> $targetRelative"
            continue
        }

        $targetDirectory = Split-Path -Parent $action.Target
        if (-not (Test-Path -LiteralPath $targetDirectory -PathType Container)) {
            New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
        }

        if ($PSCmdlet.ShouldProcess($targetRelative, "Promote pending SQL update from $sourceRelative")) {
            Move-Item -LiteralPath $action.Source -Destination $action.Target
            Write-Host "PROMOTED: $sourceRelative -> $targetRelative"
        }
    }
}

$repoRoot = Resolve-RepoRoot -RequestedRoot $Root
$promotionDateText = Resolve-PromotionDate -RequestedDate $PromotionDate
$pendingFiles = @(Get-PendingSqlFiles -RootPath $repoRoot -RequestedDatabases $Database -RequestedPaths $InputPath)

if ($pendingFiles.Count -eq 0) {
    Write-Host 'No pending SQL updates found.'
    exit 0
}

$plan = @(New-PromotionPlan -RootPath $repoRoot -Files $pendingFiles -DateText $promotionDateText)
Invoke-PromotionPlan -RootPath $repoRoot -Actions $plan -ApplyChanges:$Apply

if (-not $Apply) {
    Write-Host ''
    Write-Host 'Dry-run only. Re-run with -Apply to move these files.'
}
