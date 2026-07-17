# LinguaMesh for Windows

Native Windows client for LinguaMesh, using modern ISO C++, C++/WinRT, WinUI 3, Windows App SDK, XAML, and the generated LinguaMesh Core C++ wrapper.

## Current status

This checkpoint contains repository policy, architecture, and foundation validation only. It does not contain a Visual Studio solution, Windows application, native core package, UI, MSIX, or runnable build.

## Repository boundaries

- Own Windows UI, accessibility, lifecycle, file selection, credential protection, notifications, and packaging here.
- Consume shared translation and document behavior through the generated C++ core wrapper.
- Do not implement provider HTTP clients, document codecs, or shared persistence in the Windows client.
- Keep localization source messages in `linguamesh-l10n`.

Read [GLOBAL_GOAL.md](GLOBAL_GOAL.md), [REPOSITORY_ROLE.md](REPOSITORY_ROLE.md), and [docs/architecture.md](docs/architecture.md) before contributing.

## Current validation

No Windows SDK or external dependency is required for the foundation checkpoint.

```sh
./tools/check-foundation.sh
```

Windows setup, format, lint, test, and build commands are currently unavailable because no solution or project exists. Planned commands are labeled in [docs/testing.md](docs/testing.md).

## License

MIT. See [LICENSE](LICENSE).
