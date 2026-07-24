# Architecture

## Intended client structure

The Windows client will use modern ISO C++, C++/WinRT, WinUI 3, Windows App SDK, and XAML. UI-facing state will remain native. A single tested bridge module will consume the generated C++ RAII wrapper; direct calls to the core C ABI must not spread through the application.

The Windows layer owns UI, accessibility, application lifecycle, credential resolution, file selection and leases, clipboard, drag-and-drop, notifications, theme, package identity, and locale behavior. The shared Rust core owns providers, routing, translation, documents, shared persistence, and typed command/event semantics.

Core polling and other blocking work must run on a dedicated non-UI execution context. UI state updates must marshal onto the WinUI dispatcher. RAII must cover engine handles, FFI buffers, file handles, and cancellation lifetimes.

## Current boundary

The repository now contains a C++20 reference slice and a WinUI 3 application
surface. `AppModel` owns immutable state, model selection, locale changes,
credential references, streaming output, cancellation, and typed error keys.
`FakeCoreBridge` is deterministic and network-free; the production replacement
must be the generated Core C++ wrapper. `WindowsCredentialStore` uses
`PasswordVault` and never exposes a credential value through application state.

The portable CMake target validates the model and storage contracts on Linux or
Windows. The Visual Studio project is the authoritative WinUI/MSIX build; its
SDK, UI automation, package identity, and signing checks are Windows-only.
