#### Example definition

```yaml
extends: existence
message: Consider removing '%s'
level: warning
code: false
ignorecase: true
tokens:
    - appears to be
    - arguably
```

#### Key summary

| Name | Type | Description |
| :--- | :--- | :--- |
| `append` | `bool` | Adds `raw` to the end of `tokens`, assuming both are defined. |
| `ignorecase` | `bool` | Makes all matches case-insensitive. |
| `nonword` | `bool` | Removes the default word boundaries \(`\b`\). |
| `raw` | `array` | A list of tokens to be concatenated into a pattern. |
| `tokens` | `array` | A list of tokens to be transformed into a non-capturing group. |

The most general extension point is `existence`. As its name implies, it looks for the "existence" of particular tokens.

These tokens can be anything from simple phrases \(as in the above example\) to complex regular expressions—e.g., [the number of spaces between sentences](https://github.com/testthedocs/vale-styles/blob/master/18F/Spacing.yml) and [the position of punctuation after quotes](https://github.com/testthedocs/vale-styles/blob/master/18F/Quotes.yml).

You may define the tokens as elements of lists named either `tokens` \(shown above\) or `raw`. The former converts its elements into a word-bounded, non-capturing group. For instance,

```yaml
tokens:
  - appears to be
  - arguably
```

becomes `\b(?:appears to be|arguably)\b`.

`raw`, on the other hand, simply concatenates its elements—so, something like

```yaml
raw:
  - '(?:foo)\sbar'
  - '(baz)'
```

becomes `(?:foo)\sbar(baz)`.
