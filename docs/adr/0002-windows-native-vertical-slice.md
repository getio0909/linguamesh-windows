# ADR-0002: Windows native reference slice

## Status

Accepted for the prerelease Milestone 3 preparation checkpoint.

## Decision

The Windows client uses a C++20 `AppModel` with immutable snapshots and a
unidirectional event flow. The model owns a single worker thread for Core
polling and exposes only typed translation events to WinUI. The production
bridge boundary is `CoreBridge`; the first CI-safe implementation is a local
`FakeCoreBridge` so translation, streaming, cancellation, and error mapping
can be verified without a paid provider.

Credentials are represented in application state only by `SecretRef`. The
Windows implementation uses `PasswordVault`; the portable reference tests use
an in-memory store solely to validate the contract. No credential value is
written to settings, diagnostics, or logs.

## Consequences

This checkpoint provides a real native WinUI surface and a deterministic C++
test target that can run on Linux or Windows. Generated Core C++ wrapper
integration, device-backed Credential Locker evidence, UI automation, MSIX
packaging, and signed artifacts remain separate gates until the Windows SDK
workflow completes them on a Windows runner.
