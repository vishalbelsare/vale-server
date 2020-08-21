#### Example definition

```yaml
extends: spelling
message: "Did you really mean '%s'?"
level: error
```

#### Key summary

| Name | Type | Description |
| :--- | :--- | :--- |
| `aff` | `string` | The path to a Hunspell-compatible `.aff` file. |
| `custom` | `bool` | Turn off the default filters for acronyms, abbreviations, and numbers. |
| `dic` | `string` | The  path to a Hunspell-compatible `.dic` file. |
| `filters` | `array` | An array of patterns to ignore during spell checking. |
| `ignore` | `string` | A relative path \(from `StylesPath`\) to a personal vocabulary file consisting of one word per line to ignore. |

`spelling` implements spell checking based on Hunspell-compatible dictionaries.

#### Choosing a dictionary

By default, `spelling` includes a custom, open-source
[dictionary for American English](https://github.com/errata-ai/en_US-web).
However, you may also provide your own by using the `dic` and `aff` keys:

```yaml
extends: spelling
message: "Did you really mean '%s'?"
level: error

aff: path/to/my/file.aff
dic: path/to/my/file.dic
```

The values for both `aff` and `dic` may be absolute file paths or relative to the current `StylesPath`.

#### Ignoring non-dictionary words

`spelling` offers two different ways of ignoring non-dictionary words:

1. Using *ignore* files: Ignore files are plain-text files
   that list words to be ignored during spell check (one case-insensitive entry
   per line) . For example,

   ```text title="ignore.txt"
   destructuring
   transpiler
   ```

   Here, we're instructing `spelling` to ignore both
   `[Dd]estructuring` and `[Tt]ranspiler`.

   You can name these files anything you'd like and reference them relative to
   the active `StylesPath`:

   ```yaml
   extends: spelling
   message: "Did you really mean '%s'?"
   level: error
   ignore:
     # Located at StylesPath/ignore1.txt
     - ignore1.txt
     - ignore2.txt
   ```

2. Using *filters*: You can also customize the spell-checking experience by
   defining *filters*, which are Go-compatible
   regular expressions to applied to individual words:

   ```yaml
   extends: spelling
   message: "Did you really mean '%s'?"
   level: error
   # This disables the built-in filters. If you omit this
   # key or set it to false, custom filters (see below) are
   # added on top of the built-in ones.
   #
   # By default, filters for acronyms, abbreviations, and
   # numbers are included.
   custom: true
   # A "filter" is a regular expression specifying words
   # to ignore during spell checking.
   filters:
     # Ignore all words starting with 'py'.
     #
     # e.g., 'PyYAML'.
     - '[pP]y.*\b'
   ```
