function writeToConsole (msg, isError) {
  $('.log').empty()
  $('<div/>', {
    class: isError ? 'status error' : 'status',
    text: msg
  }).appendTo('.log')
}
