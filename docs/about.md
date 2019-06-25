---
id: about
title: What is Vale Server?
---

## Vale Server is a *linter* for prose

[Vale Server](https://errata.ai/vale-server/) is a cross-platform
(macOS and Windows) desktop application built on top of
[Vale](https://github.com/errata-ai/vale), an open-source linter for prose.

:::important Definition
[*Linting*](https://en.wikipedia.org/wiki/Lint_(software)) is the process of ensuring that written work (code or prose) adheres to a particular *style*&mdash;for example, Python’s [PEP 8 style guide](https://www.python.org/dev/peps/pep-0008/#introduction) (code) or [The Chicago Manual of Style](https://www.chicagomanualofstyle.org/home.html) (prose).
:::

In other words, Vale Server is not a general-purpose writing aid. It doesn’t
teach you *how* to write; it’s a tool *for* writers. More
specifically, Vale Server focuses on the *style* of writing rather than its
grammatical correctness&mdash;making it fundamentally different from, for example,
Grammarly.


## Your style, our editor

Vale Server works by enforcing *rules* (see [Styles 101](style) for more
information) that are defined in simple [YAML](https://yaml.org/) files:

```yaml
# This is an example rule that ensures that all headings follow
# sentence-style capitalization.
extends: capitalization
message: "'%s' should use sentence-style capitalization."
level: suggestion
scope: heading
match: $sentence
exceptions:
  - Azure
  - CLI
...
```

Multiple rules are packaged together to form *styles* that allow you to ensure
that your content is clear, consistent, and on-brand. You can create your own
or choose one from the [styles library](https://github.com/errata-ai/styles).

## Private and secure

Vale Server's architecture is different from most writing applications and
services: since it has an open-source core, the server-side logic is bundled
*with* the application and runs on `localhost`&mdash;meaning your content never
has to leave your computer.
