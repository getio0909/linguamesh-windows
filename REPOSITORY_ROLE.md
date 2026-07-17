# Repository Role

`linguamesh-windows` is the native Windows client repository for LinguaMesh.

It is responsible for:

- modern C++, C++/WinRT, WinUI 3, Windows App SDK, and XAML UI;
- Windows navigation, accessibility, lifecycle, theme, and locale behavior;
- Credential Locker or current-user-scoped credential protection;
- native file leases, clipboard, drag-and-drop, and notifications;
- generated C++ core-wrapper consumption and RAII ownership;
- MSIX and documented optional portable packaging.

Provider adapters, routing, translation rules, document codecs, shared SQLite data, and command/event semantics belong to `linguamesh-core`. Canonical UI messages belong to `linguamesh-l10n`.

The current foundation does not claim a buildable or runnable Windows application.
