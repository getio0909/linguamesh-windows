#!/usr/bin/env python3
"""Check the Windows native slice without requiring a Windows host."""

from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[1]
REQUIRED = (
    "CMakeLists.txt",
    "LinguaMesh.Windows.sln",
    "LinguaMesh.Windows.vcxproj",
    "Package.appxmanifest",
    "Assets/LinguaMesh.svg",
    "include/linguamesh/windows/app_model.hpp",
    "include/linguamesh/windows/core_bridge.hpp",
    "include/linguamesh/windows/credential_store.hpp",
    "src/app/app_model.cpp",
    "src/core/core_bridge.cpp",
    "src/platform/windows_credential_store.cpp",
    "src/winui/App.xaml",
    "src/winui/MainWindow.xaml",
    "docs/adr/0002-windows-native-vertical-slice.md",
    "tests/core_wrapper_smoke.cpp",
    "tools/test-pinned-core-wrapper.ps1",
)


def main() -> int:
    missing = [path for path in REQUIRED if not (ROOT / path).is_file()]
    if missing:
        for path in missing:
            print(f"Missing Windows slice file: {path}", file=sys.stderr)
        return 1

    if list(ROOT.glob("**/*.csproj")):
        print("C# production projects are not permitted.", file=sys.stderr)
        return 1

    for path in ROOT.glob("include/**/*.hpp"):
        text = path.read_text(encoding="utf-8")
        if "password" in text.lower() or "secret-canary" in text.lower():
            print(f"Secret values must not be represented in public headers: {path}", file=sys.stderr)
            return 1

    print("Windows native source validation passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
