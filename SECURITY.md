# Security Policy

## Reporting a vulnerability

Do not open a public issue for an unpatched vulnerability. Use the private security-reporting channel configured on the canonical GitHub repository. If that channel is unavailable, contact the maintainers privately before disclosing details.

Include affected revision, Windows version, package mode, reproduction conditions, impact, and a minimal non-sensitive proof. Never include credentials, private source text, user documents, or personal data.

## Windows security requirements

- Protect credentials with Credential Locker or current-user-scoped OS data protection.
- Never store plaintext API keys in settings, registry values, logs, diagnostics, crash data, or package state.
- Use RAII around all native handles, C ABI buffers, and secret lifetimes.
- Treat provider output, imported settings, activation data, locale data, and document content as untrusted.
- Require HTTPS for remote endpoints; permit HTTP only for explicitly configured loopback development endpoints.
- Keep signing material and production credentials out of source and public-fork CI.

No release is currently supported. Security support windows will be documented with the first published application version.
