# Security Policy

This document outlines how security vulnerabilities should be reported, handled, and disclosed for **SysMood**.

---

## Supported Versions

We actively maintain only the **latest stable release** of SysMood.  
Older versions do **not** receive security patches.

| Version | Supported |
|----------|------------|
| Latest |  Yes |
| Older releases |  No |

Please upgrade to the latest version before reporting any issues.

---

## Reporting a Vulnerability

If you discover a vulnerability or security risk in SysMood, **do not open a public GitHub issue**.

Instead, please:

1. **Email the maintainer directly**  

2. Provide as much detail as possible:
   - Description of the issue  
   - Steps to reproduce (if available)  
   - Potential impact or exploit scenario  
   - Any suggested fix or mitigation  

We aim to acknowledge your report **within 72 hours**, and will work with you to validate, fix, and coordinate disclosure responsibly.

---

## Responsible Disclosure Guidelines

Please adhere to the following principles:

- **Do not publicly disclose** the vulnerability before a fix has been released.  
- **Avoid disruptive testing** — do not run automated scans or denial-of-service attempts against public systems.  
- **Act in good faith** to protect end users and data.  

Following these practices helps keep the community safe and ensures your findings are recognized and properly credited.

---

## Disclosure Policy

We follow a **coordinated disclosure** approach:

- Once a verified fix is released, details of the vulnerability may be publicly disclosed.  
- Credit will be given to the reporter (if requested).  
- Until that time, all vulnerability details must remain private.

---

## Security Best Practices

Because SysMood interacts with system-level data (CPU, memory, process states), users and contributors should follow these best practices:

### For Users
- Run SysMood in a **trusted and isolated environment**.  
- Avoid executing the binary with **administrator/root privileges** unless strictly necessary.  
- **Verify release integrity** — check provided SHA-256 checksums or signatures.  
- Keep your **compiler and dependencies** (if any) up to date.  
- Do not share or include sensitive data (API keys, credentials, etc.) in logs or builds.

### For Contributors
- Use **safe C++ practices**: bounds checking, smart pointers, input validation.  
- Enable **compiler warnings and sanitizers** (`-Wall -Wextra -fsanitize=address,undefined`).  
- Avoid unsafe C-style memory operations and unchecked parsing.  
- Review commits for potential security regressions (especially if adding file or system access features).  
- Use GitHub’s **CodeQL** or **Clang-Tidy** for static analysis when possible.

---

## Specific Risk Areas & Mitigations

| Risk | Description | Mitigation |
|------|--------------|-------------|
| **Privilege escalation** | Running with elevated permissions may expose system data. | Run with least privilege. Document any required permissions. |
| **Memory safety** | Buffer overflows or invalid reads in native code. | Use safe libraries, smart pointers, sanitizers, and code review. |
| **Output injection** | Unsanitized console output or file writes. | Sanitize dynamic strings and avoid interpreting user input as commands. |
| **Sensitive logging** | Logging internal system info or PII. | Avoid unnecessary logging; redact sensitive details. |
| **Binary integrity** | Users running tampered executables. | Publish checksums or signatures for official releases. |

---

## Acknowledgments

We sincerely thank community members who report vulnerabilities responsibly.  
Your help makes SysMood safer for everyone.

---

## Resources

- [GitHub Security Advisories](https://docs.github.com/en/code-security/security-advisories)
- [OpenSSF Best Practices](https://bestpractices.dev/)
- [OWASP Top 10 (for Native Applications)](https://owasp.org/www-project-top-ten/)
- [Microsoft Secure Coding Guidelines for C++](https://learn.microsoft.com/en-us/cpp/security/secure-coding-guidelines)

---
