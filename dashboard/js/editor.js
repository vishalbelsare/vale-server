let editor = null
let output = null
let langid = 'en'

$(document).ready(function () {
    editor = CodeMirror($('#editor').get(0), ymlConfig)

    output = CodeMirror($('#results').get(0), mdConfig)
    output.on('change', function (cm, change) {
        clear(cm)
    })

    $('#run').click(function (e) {
        e.preventDefault()
        output.performLint()
    })

    $('#compile').click(function (e) {
        e.preventDefault()
        compile(editor.getValue(), output.getValue())
    })

    $('input[name="options"]').change(function () {
        const option = $(this).attr('id')
        if (option === 'option2') {
            tag(output, langid)
        } else {
            clearTags(output)
        }
    })

    $(document).ajaxSend(function (event, request, settings) {
        writeToConsole('Processing ...', false)
    })

    const darkSwitch = document.getElementById('darkSwitch')
    if (darkSwitch) {
        initTheme()
        darkSwitch.addEventListener('change', function (event) {
            resetTheme()
        })

        function initTheme() {
            const darkThemeSelected =
                localStorage.getItem('darkSwitch') !== null &&
                localStorage.getItem('darkSwitch') === 'dark'

            darkSwitch.checked = darkThemeSelected
            switchTheme(editor, output, darkThemeSelected)
        }

        function resetTheme() {
            switchTheme(editor, output, darkSwitch.checked)
        }
    }
})
