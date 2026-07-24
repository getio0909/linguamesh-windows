# Testing

## Portable reference checks

Setup requires only Bash, Git, and standard POSIX utilities. From the repository root run:

```sh
./tools/check-foundation.sh
python3 tools/check-windows-source.py
cmake -S . -B out/reference -DCMAKE_BUILD_TYPE=Release
cmake --build out/reference
ctest --test-dir out/reference --output-on-failure
```

The first command verifies the repository foundation; the second validates the
native Windows source boundary; the CMake target builds and runs the deterministic
streaming, cancellation, typed-error, locale, and SecretRef tests without a
provider credential.

## Windows SDK checks

On a Windows 11 runner with Visual Studio 2022, Windows App SDK, and the Windows
10.0.26100 SDK, run:

```powershell
msbuild LinguaMesh.Windows.sln /restore /p:Configuration=Debug /p:Platform=x64
msbuild LinguaMesh.Windows.sln /t:ClCompile /p:Configuration=Debug /p:Platform=x64 /p:RunCodeAnalysis=true
msbuild LinguaMesh.Windows.sln /p:Configuration=Release /p:Platform=x64
```

The Windows workflow runs the portable tests and the Debug/Release project
builds. The native job also checks out the immutable Core and runs
`tools/test-pinned-core-wrapper.ps1`, which builds the pinned Rust FFI and
executes the Windows C++20 RAII wrapper smoke before the WinUI build. UI
automation, high contrast, keyboard access, and MSIX runtime smoke tests remain
explicit evidence gates; a successful CMake or wrapper smoke is not evidence
for those gates.
