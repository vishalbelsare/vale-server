# Vale Server

The goal of the Vale Server project is to make it possible to integrate [Vale](https://github.com/errata-ai/vale) with applications that run in a [sandboxed](https://en.wikipedia.org/wiki/Sandbox_(computer_security)) (or otherwise restrictive) environment (e.g., Google Docs, Microsoft Word, and web browsers) without sacrificing performance, privacy, or extensibility.

## Why is this necessary?

Most "similar" software (e.g., Grammarly, LanguageTool, ProWritingAid, etc.) expose their functionality through a public API that other applications can integrate into their own services either directly or through an extension system:

```text
3rd party app <-> Public API <-> Private remote server
```

However, there are a number of downsides that make this approach incompatible with Vale and its goals:

- Performance: Most services that rely on remote servers have limitations on the amount of text that can be checked at once. This would simply be a deal-breaking for Vale (which is fast enough to run [on entire repositories](https://medium.com/p/63c4de31be00#fa9c)) because handling reasonably large documents is one of its core features.
- Extensibility: Vale, unlike most of its counterparts, offers a number of ways to control and extend its functionality. In order to do so, though, Vale needs to access local YAML, ini, and Go files. This would mean that either these files would need to be sent with every request to a remote server (resource-intensive) or the remote server would need local file access (a security risk).
- Privacy: Putting Vale behind an API would raise a number of privacy and security risks that it has been able to avoid: its cross-platform, standalone binaries allow it to come to you, rather than forcing you to send your text to some remote server.

## How does this work?

Now that we've established why the traditional server architecture isn't a good fit for Vale, let's discuss how the Vale Server project intends to address these issues.
