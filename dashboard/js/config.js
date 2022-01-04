function clear (cm) {
  if (cm) {
    cm.clearGutter('CodeMirror-lint-markers')
    cm.getDoc().getAllMarks().forEach((marker) => marker.clear())
  }
}

function diableRunStatus (status) {
  $('#run').prop('disabled', status)
  $('#compile').prop('disabled', status)
}

const ymlConfig = {
  lineNumbers: true,
  lineWrapping: true,
  indentWithTabs: false,
  indentUnit: 2,
  tabSize: 2,
  value: '---\nextends: substitution\n\
message: "Use \'%s\' instead of \'%s\'"\n\
level: error\n\
ignorecase: false\n\
# swap maps tokens in form of bad: good\n\
swap:\n\
  regex101: Regex101\n\
  accelerate: speed up\n\
',
  mode: 'yaml',
  theme: 'github',
  gutters: ['CodeMirror-lint-markers'],
  lint: {
    getAnnotations: asyncCompile,
    async: true
  }
}
const mdConfig = {
  lineNumbers: true,
  lineWrapping: true,
  value: '# Vale Studio \n\n\
Vale Studio allows you to easily create, test, and debug new rules -- helping you accelerate your development workflow. \n\n\
Click the "Run" button above to test the rule in left-hand editor on this text. \n\n\
## Regex101 \n\n\
```python \n\
print("Hello, world!") \n\
```\n\n\
Vale Studio comes with **built-in** integration with [*regex101*](https://regex101.com/), allowing you to \
test the regular expression Vale uses after compiling the YAML-based rule.\
',
  mode: 'markdown',
  theme: 'github',
  gutters: ['CodeMirror-lint-markers', 'gutter-error'],
  lint: {
    getAnnotations: asyncLint,
    async: true,
    lintOnChange: false
  }
}

const guidedTour = {
  name: 'tourist',
  steps: [
    {
      element: '#main-nav',
      title: 'Welcome to Vale Studio!',
      content: 'This short tour will walk you through the UI and its features.',
      placement: 'bottom'
    },
    {
      element: '#run',
      title: 'Compiling a Rule',
      content:
          'Clicking the \'Compile\' button will build your rule (left-hand editor) and test it against the provided Markdown (right-hand editor).'
    },
    {
      element: '#log',
      title: 'Logging Output',
      content:
          'All syntax and run-time errors will be reported in the console below.',
      placement: 'top'
    },
    {
      element: '#tags-view',
      title: 'View: Tags',
      content:
          'The \'View: Tags\' option switches from a plain-text view to a tokenized view with associated NLP information.',
      placement: 'bottom'
    },
    {
      element: '#compile',
      title: 'Regex101 Integration',
      content:
          'The \'code\' button will launch a Regex101 session that\'s pre-populated with the provided test markup and the compiled rule\'s regex.',
      placement: 'left'
    }
  ],
  debug: false, // you may wish to turn on debug for the first run through
  framework: 'bootstrap4' // set Tourist to use BS4 compatibility
}
