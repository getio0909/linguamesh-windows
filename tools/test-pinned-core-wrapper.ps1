[CmdletBinding()]
param(
    [string]$CoreRoot = "linguamesh-core",
    [string]$ExpectedCoreRevision = ""
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$CorePath = (Resolve-Path (Join-Path $RepoRoot $CoreRoot)).Path
$CoreRevision = (& git -C $CorePath rev-parse HEAD).Trim()
if ($ExpectedCoreRevision -and $CoreRevision -ne $ExpectedCoreRevision) {
    throw "Pinned Core revision mismatch: expected $ExpectedCoreRevision, found $CoreRevision."
}

Push-Location $CorePath
try {
    & cargo build --locked --release -p linguamesh-ffi
    if ($LASTEXITCODE -ne 0) {
        throw "Pinned Core FFI release build failed."
    }
}
finally {
    Pop-Location
}

$ReleaseRoot = Join-Path $CorePath "target\release"
$DllPath = Join-Path $ReleaseRoot "linguamesh_ffi.dll"
$ImportCandidates = @(
    (Join-Path $ReleaseRoot "linguamesh_ffi.dll.lib"),
    (Join-Path $ReleaseRoot "linguamesh_ffi.lib")
)
$ImportLibrary = $ImportCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not (Test-Path $DllPath) -or $null -eq $ImportLibrary) {
    throw "Pinned Core DLL or import library was not found."
}

$NativeTestDir = Join-Path ([System.IO.Path]::GetTempPath()) ("linguamesh-windows-core-" + [guid]::NewGuid())
New-Item -ItemType Directory -Path $NativeTestDir | Out-Null
try {
    $Executable = Join-Path $NativeTestDir "core_wrapper_smoke.exe"
    $Source = Join-Path $RepoRoot "tests\core_wrapper_smoke.cpp"
    $AbiInclude = Join-Path $CorePath "contracts\abi"
    $CppInclude = Join-Path $CorePath "bindings\cpp\include"
    $Object = Join-Path $NativeTestDir "core_wrapper_smoke.obj"
    & cl.exe "/nologo" "/std:c++20" "/EHsc" "/W4" "/WX" "/I$AbiInclude" "/I$CppInclude" `
        $Source $ImportLibrary "/Fo:$Object" "/Fe:$Executable"
    if ($LASTEXITCODE -ne 0) {
        throw "Pinned Core C++ wrapper smoke compilation failed."
    }
    Copy-Item $DllPath (Join-Path $NativeTestDir "linguamesh_ffi.dll")
    & $Executable
    if ($LASTEXITCODE -ne 0) {
        throw "Pinned Core C++ wrapper smoke execution failed."
    }
    Write-Output "Pinned Core C++ wrapper validation passed."
}
finally {
    Remove-Item -Recurse -Force $NativeTestDir
}
