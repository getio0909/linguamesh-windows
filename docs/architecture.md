# Architecture

## Intended client structure

The Windows client will use modern ISO C++, C++/WinRT, WinUI 3, Windows App SDK, and XAML. UI-facing state will remain native. A single tested bridge module will consume the generated C++ RAII wrapper; direct calls to the core C ABI must not spread through the application.

The Windows layer owns UI, accessibility, application lifecycle, credential resolution, file selection and leases, clipboard, drag-and-drop, notifications, theme, package identity, and locale behavior. The shared Rust core owns providers, routing, translation, documents, shared persistence, and typed command/event semantics.

Core polling and other blocking work must run on a dedicated non-UI execution context. UI state updates must marshal onto the WinUI dispatcher. RAII must cover engine handles, FFI buffers, file handles, and cancellation lifetimes.

## Current boundary

No solution, project, wrapper, or runtime code exists at this checkpoint. This document defines constraints and does not claim a Windows implementation.
