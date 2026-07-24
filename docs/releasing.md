# Releasing

The WinUI project now has an unsigned MSIX packaging definition, but no release
artifact is claimed until the Windows workflow builds it from immutable Core and
localization inputs.

A release must pin compatible core native artifacts, ABI, protocol,
provider-catalog, and localization versions. CI must run formatting, static
analysis, native unit and wrapper tests, UI automation, localization validation,
compatibility checks, release builds, dependency/license review, and MSIX smoke
tests.

Produce checksummed MSIX artifacts and, only when documented, an optional portable package. Keep signing credentials outside source and public-fork CI, and do not claim an artifact is signed unless verified. Record application versions and checksums in the central `linguamesh-project/release-manifest.toml`. Mark prereleases clearly and never publish an unverified stable version.
