# LinguaMesh for Windows

Native Windows client for LinguaMesh, using modern ISO C++, C++/WinRT, WinUI 3, Windows App SDK, XAML, and the generated LinguaMesh Core C++ wrapper.

## Current status

The repository contains the first native WinUI 3 surface and a deterministic
C++20 reference slice. The slice discovers fake models, streams output,
cancels while retaining partial output, switches locale, maps typed errors, and
stores credentials behind a `SecretRef`. The fake bridge is intentionally
network-free; generated Core C++ wrapper integration and live-provider work are
separate compatibility gates.

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
python3 tools/check-windows-source.py
cmake -S . -B out/reference -DCMAKE_BUILD_TYPE=Release
cmake --build out/reference
ctest --test-dir out/reference --output-on-failure
```

The Visual Studio/MSIX commands and their evidence boundary are documented in
[docs/testing.md](docs/testing.md).

## License

MIT. See [LICENSE](LICENSE).
