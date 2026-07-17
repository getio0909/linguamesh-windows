# LinguaMesh Windows Instructions

## Required reading

Before changing this repository, read `REPOSITORY_ROLE.md`, `GLOBAL_GOAL.md`, `IMPLEMENTATION_STATUS.md`, and the relevant files under `docs/`.

## Scope

This repository owns the native Windows client. Use modern ISO C++, C++/WinRT, WinUI 3, Windows App SDK, XAML where appropriate, and the generated LinguaMesh Core C++ wrapper. Do not add a C# production application layer, shared UI framework, provider networking, document parsing, or direct C ABI calls outside the future wrapper module.

## Workflow

1. Inspect `git status --short` and preserve user changes.
2. Confirm the pinned global-goal and core compatibility revisions.
3. Record uncertain decisions with `Assumption:`.
4. Implement the smallest complete native vertical slice with tests.
5. Run every available command in `docs/testing.md`.
6. Update `IMPLEMENTATION_STATUS.md` with exact evidence.

## Windows rules

- Use RAII for Win32 handles, C ABI buffers, and core engine lifetime.
- Keep core polling, network, database, and document work off the WinUI dispatcher thread.
- Use Windows Credential Locker or current-user-scoped OS data protection; never use plaintext settings or registry values for secrets.
- Use supported native file pickers and document file leases.
- Preserve high contrast, text scaling, keyboard access, screen readers, system theme, and package-identity behavior.
- Use generated resources from `linguamesh-l10n` and preserve runtime locale switching.
- All code comments must be Simplified Chinese on separate lines above the code they describe.
- All console, log, diagnostic, and command-line output strings must be English.

Do not claim build, MSIX packaging, accessibility, credential protection, or ABI compatibility without tests and reproducible Windows evidence.
