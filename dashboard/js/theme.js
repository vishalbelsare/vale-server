
function switchTheme (editor, output, toDark) {
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

$(document).ready(function () {
  const darkSwitch = document.getElementById('darkSwitch')
  if (darkSwitch) {
    initTheme()
    darkSwitch.addEventListener('change', function (event) {
      resetTheme()
    })
    function initTheme () {
      const darkThemeSelected =
                localStorage.getItem('darkSwitch') !== null &&
                localStorage.getItem('darkSwitch') === 'dark'

      darkSwitch.checked = darkThemeSelected
      switchTheme(editor, output, darkThemeSelected)
    }
    function resetTheme () {
      switchTheme(editor, output, darkSwitch.checked)
    }
  }
})
