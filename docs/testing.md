# Testing

## Available now

Setup requires only Bash, Git, and standard POSIX utilities. From the repository root run:

```sh
./tools/check-foundation.sh
```

This verifies required foundation files, the global-goal revision pin, repository identity, line endings, and trailing whitespace. It is the only implemented format/lint/test check. There is no Windows build.

## Planned commands, unavailable now

The following command contract is planned but cannot run until `LinguaMesh.sln`, its projects, and a pinned Windows toolchain exist:

```powershell
msbuild LinguaMesh.sln /restore /p:Configuration=Debug /p:Platform=x64
msbuild LinguaMesh.sln /t:ClCompile /p:Configuration=Debug /p:Platform=x64 /p:RunCodeAnalysis=true
vstest.console.exe .\out\x64\Debug\LinguaMesh.Tests.dll
msbuild LinguaMesh.sln /p:Configuration=Release /p:Platform=x64
```

Do not report any of these commands as executed at the foundation checkpoint. Wrapper ownership, UI automation, high contrast, keyboard access, ABI compatibility, and MSIX smoke-test commands must be added when their projects exist.
