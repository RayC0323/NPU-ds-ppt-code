param(
    [switch]$Run,
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $Root "build"
$RootFull = [System.IO.Path]::GetFullPath($Root)
if (-not $RootFull.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
    $RootFull += [System.IO.Path]::DirectorySeparatorChar
}
$RootUri = [Uri]$RootFull

if ($Clean -and (Test-Path -LiteralPath $BuildDir)) {
    Remove-Item -LiteralPath $BuildDir -Recurse -Force
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

$sources = Get-ChildItem -LiteralPath $Root -Recurse -Filter "*.c" |
    Where-Object { $_.FullName -notmatch "\\build\\" }

foreach ($source in $sources) {
    $SourceUri = [Uri]([System.IO.Path]::GetFullPath($source.FullName))
    $relative = [Uri]::UnescapeDataString($RootUri.MakeRelativeUri($SourceUri).ToString())
    $relative = $relative.Replace('/', [System.IO.Path]::DirectorySeparatorChar)
    $relativeNoExt = $relative -replace "\.c$", ""
    $binaryName = $relativeNoExt -replace "[\\/]", "_"
    if ($env:OS -eq "Windows_NT") {
        $binaryName = "$binaryName.exe"
    }
    $output = Join-Path "build" $binaryName
    $outputAbs = Join-Path $Root $output
    Write-Host "gcc $relative"
    Push-Location $Root
    try {
        & gcc -std=c11 -Wall -Wextra -pedantic $relative -o $output
        if ($LASTEXITCODE -ne 0) {
            throw "compile failed: $relative"
        }
    }
    finally {
        Pop-Location
    }
    if ($Run -and $relative -notmatch "^src[\\/]") {
        Write-Host "== $relative =="
        & $outputAbs
        if ($LASTEXITCODE -ne 0) {
            throw "run failed: $relative"
        }
    }
}
