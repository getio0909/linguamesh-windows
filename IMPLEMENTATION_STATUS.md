# Implementation Status

## 2026-07-24 — Windows C++/WinRT reference slice checkpoint

Assumption: the current Windows slice is a bounded prerelease reference surface; it proves
portable state/event and SecretRef contracts locally, while WinUI/MSIX and Windows-only
integration remain hosted evidence gates.

- Windows code head `69cad67dbec2743a2ce91d9344fd916ebb201012` contains the native C++20
  `AppModel`, deterministic fake bridge, PasswordVault adapter, WinUI/XAML shell, IDL runtime
  class, and portable reference tests. The fake bridge is network-free and never requires a
  paid credential.
- Local Foundation/source-boundary checks, CMake Release build, CTest (1/1), and `git diff --check`
  pass. Hosted push run `30090288192` passed both the portable C++ job and the WinUI/MSIX job;
  Foundation run `30090288187` passed.
- Hosted Debug and Release WinUI builds compiled and linked, generated XAML/PRI resources, and
  completed unsigned MSIX preparation. The run emitted only existing toolchain warnings (WinUI
  `GetCurrentTime` macro, missing ATL library path, and the GitHub Actions Node 20 notice).
- This proves build and packaging preparation only; it does not claim Windows UI runtime,
  UI automation, Credential Locker/Password Vault behavior, accessibility, signing, or generated
  Core-wrapper conformance.
- Provider profiles, document workflows, routing, history, cross-client parity, and stable
  release remain open; release status stays `unreleased`.

## 2026-07-24 — Windows native reference vertical slice

Assumption: the first Windows checkpoint can verify the native state/event and
secure-storage contracts with a deterministic local provider while the
Windows-only SDK and UI automation gates run on the hosted Windows runner.
The fake bridge never sends network traffic or requires a paid credential.

### Implemented

- Added a C++20 `AppModel` with immutable snapshots, model discovery/manual
  selection, locale switching, streaming output, cancellation with partial
  output retention, and typed localized error keys.
- Added a `CoreBridge` boundary and deterministic fake provider with exactly one
  terminal event. The future generated Core C++ wrapper replaces this bridge
  without moving provider or persistence code into the UI.
- Added `SecretRef`-only credential ownership. The Windows implementation uses
  `PasswordVault`; the portable test store is memory-only and is never used as a
  production persistence path.
- Added a WinUI 3/XAML shell, Visual Studio solution, MSIX manifest, and a
  pinned Windows App SDK project definition. Core work remains off the UI
  dispatcher thread.
- Added portable CMake tests plus a Windows workflow that checks the source
  boundary, builds the reference slice, and runs Debug/Release WinUI builds.

### Evidence

- `python3 tools/check-windows-source.py` passed with
  `Windows native source validation passed.`
- `cmake -S . -B out/reference -DCMAKE_BUILD_TYPE=Release`,
  `cmake --build out/reference --parallel`, and
  `ctest --test-dir out/reference --output-on-failure` passed: one test target,
  100% passed.
- `git diff --check` passed.

Windows-host MSBuild, WinUI runtime, UI automation, Credential Locker/Password
Vault, accessibility, MSIX output, signing, and generated Core-wrapper
conformance are not claimed until the Windows workflow completes them. Provider
profiles, document workflows, routing, history, and stable release remain open.

## Historical foundation checkpoint — 2026-07-17

### Initially implemented

- Repository policies and Windows role boundaries.
- Pinned reference to the authoritative global goal.
- Foundation-only validation script and GitHub Actions workflow.
- Architecture, testing, and release guidance.

### Initially not implemented

- Visual Studio solution, Windows application projects, or package identity.
- WinUI 3 UI, navigation, accessibility, theme, or locale switching.
- Generated core wrapper, native core package, or compatibility negotiation.
- Credential broker, file broker, notifications, or lifecycle integration.
- Native unit, wrapper, UI automation, accessibility, or packaging tests.
- MSIX, portable package, signing, or release workflow.

### Initial evidence

Validated locally on 2026-07-17:

- `bash -n tools/check-foundation.sh` exited successfully.
- `./tools/check-foundation.sh` exited successfully with `Foundation validation passed.`
- `git branch --show-current` returned `main`.
- MSBuild, native static analysis, VSTest, and Windows build commands were not run because no solution or Windows project exists and this host is not Windows.
- Files remain uncommitted and unstaged for the coordinating repository to review.
