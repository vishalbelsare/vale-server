var API = "http://localhost:" + location.port;

function clearTags(cm) {
    cm.setOption('readOnly', false)
    cm.getDoc()
        .getAllMarks()
        .forEach(function (marker) {
            if (marker.attributes !== undefined) {
                marker.clear()
            }
        })
}

function switchTheme(editor, output, toDark) {
    if (toDark) {
        $('#main-nav').removeClass('navbar-light')
        $('#main-nav').removeClass('bg-white')
        document.body.setAttribute('data-theme', 'dark')
        $('#main-nav').addClass('navbar-dark')
        $('#main-nav').addClass('bg-dark')
        localStorage.setItem('darkSwitch', 'dark')
        editor.setOption('theme', 'github-dark')
        output.setOption('theme', 'github-dark')
    } else {
        $('#main-nav').removeClass('navbar-dark')
        $('#main-nav').removeClass('bg-dark')
        document.body.removeAttribute('data-theme')
        $('#main-nav').addClass('navbar-light')
        $('#main-nav').addClass('bg-white')
        localStorage.removeItem('darkSwitch')
        editor.setOption('theme', 'github')
        output.setOption('theme', 'github')
    }
}

function upload(regex, text) {
    $.ajax({
        type: 'POST',
        url: API + "/upload",
        datatype: 'json',
        data: {
            regex: regex,
            text: text
        },
        success: function (resp) {
            writeToConsole('Syntax OK; rule compiled successfully.', false)
            window.open('https://regex101.com/r/' + resp.permalinkFragment, '_blank')
        },
        error: function (xhr, status, text) {
            writeToConsole('[server error] ' + xhr.responseText, true)
        }
    })
}

function compile(rule, text) {
    $.ajax({
        type: 'POST',
        url: API + "/compile",
        datatype: 'json',
        data: {
            code: rule
        },
        success: function (data) {
            upload(data.Pattern, text)
        },
        error: function (xhr, status, text) {
            writeToConsole('[server error] ' + xhr.responseText, true)
        }
    })
}

function skipLine(doc, n) {
    const hand = doc.getLineHandle(n)
    if (hand.styles.slice(-1)[0] === 'comment') {
        return true
    }

    const line = doc.getLine(n)
    return ['#', '-', '*'].some(function (v) {
        return line.startsWith(v)
    })
}

function tag(cm, langid) {
    $.ajax({
        type: 'POST',
        url: API + "/tag",
        datatype: 'json',
        data: {
            text: cm.getValue(),
            lang: langid,
            endp: ""
        },
        success: function (ctx) {
            const doc = cm.getDoc()

            clearTags(cm)
            for (let idx = 0; idx < ctx.length; idx++) {
                const node = ctx[idx]
                if (skipLine(doc, node.Line - 1)) {
                    continue
                }

                doc.markText({
                    line: node.Line - 1,
                    ch: node.Span[0] - 1
                }, {
                    line: node.Line - 1,
                    ch: node.Span[1]
                }, {
                    className: 'nlp-token',
                    attributes: {
                        'data-entity': node.Token.Tag
                    }
                })

                cm.setOption('readOnly', 'nocursor')
                writeToConsole('Syntax OK; rule compiled successfully.', false)
            }
        },
        error: function (xhr, status, text) {
            writeToConsole('[server error] ' + xhr.responseText, true)
        }
    })
}

function asyncCompile(text, updateLinting, options) {
    clear(output)
    if (text.trim() == '') {
        updateLinting([])
        return
    }

    $.ajax({
        url: API + "/compile",
        method: 'POST',
        data: {
            code: text
        },
        success: function (error) {
            const found = []
            try {
                const errorLine = error.Line - 1
                found.push({
                    from: CodeMirror.Pos(errorLine, error.Span - 1),
                    to: CodeMirror.Pos(errorLine),
                    message: error.Text
                })

                const alert =
                    '[' +
                    error.Line.toString() +
                    ':' +
                    error.Span.toString() +
                    ']:' +
                    ' ' +
                    error.Text

                diableRunStatus(true)
                writeToConsole('Syntax error ' + alert, true)
            } catch (error) {
                diableRunStatus(false)
                writeToConsole('Syntax OK; rule compiled successfully.', false)
            }
            updateLinting(found)
        },
        error: function (xhr, status, text) {
            console.log(xhr, text, status)
            writeToConsole('[server error] ' + xhr.responseText, true)
        }
    })
}

function asyncLint(text, updateLinting, options) {
    if (text.trim() == '') {
        updateLinting([])
        return
    }

    $.ajax({
        url: API + "/run",
        method: 'POST',
        data: {
            code: editor.getValue(),
            text: text
        },
        success: function (errors) {
            const found = []

            for (const key in errors) {
                for (let i = 0; i < errors[key].length; i++) {
                    const error = errors[key][i]
                    if ('Action' in error) {
                        const errorLine = error.Line - 1

                        found.push({
                            from: CodeMirror.Pos(errorLine, error.Span[0] - 1),
                            to: CodeMirror.Pos(errorLine, error.Span[1]),
                            message: error.Message
                        })
                    }
                }
            }

            writeToConsole('Syntax OK; rule compiled successfully.', false)
            updateLinting(found)
        },
        error: function (xhr, status, text) {
            writeToConsole('[server error] ' + xhr.responseText, true)
            console.log(text, status)
        }
    })
}
