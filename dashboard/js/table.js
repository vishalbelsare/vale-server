const API = 'http://localhost:' + location.port
const reZeroWidthWhite = new RegExp('\u2028|\u2029|\xe2\x80\x8b')

const $table = $('#table')
const $add = $('#form')
const $remove = $('#remove')

const selections = []

function addTerm (term, file) {
  const name = $('#vocab').find(':selected').text()
  $.getJSON(API + '/vocab?name=' + name + '&file=' + file, function (data) {
    data.push(term.replace(reZeroWidthWhite, ''))
    console.log('Updated', data)
    $.ajax({
      type: 'POST',
      url: API + '/update',
      datatype: 'json',
      data: {
        path: name + '.' + file,
        text: data.join('\n')
      },
      success: function (data, status, xhr) {
        console.log(data)
      },
      error: function (error) {
        console.log(error)
      }
    })
  })
}

function deleteTerms (entries, file) {
  const name = $('#vocab').find(':selected').text()

  $.getJSON(API + '/vocab?name=' + name + '&file=' + file, function (data) {
    for (let i = 0; i < entries.length; i++) {
      const index = data.indexOf(entries[i].term)
      if (index > -1) {
        data.splice(index, 1)
      }
    }

    $.ajax({
      type: 'POST',
      url: API + '/update',
      datatype: 'json',
      data: {
        path: name + '.' + file,
        text: data.join('\n')
      },
      success: function (data, status, xhr) {
        console.log(data)
      },
      error: function (error) {
        console.log(error)
      }
    })
  })
}

function initTable () {
  const table = new Tabulator('#table', {
    layout: 'fitColumns',
    placeholder: 'No Vocab loaded.',
    pagination: 'local',
    paginationSize: 10,
    columns: [
      { title: 'Term', field: 'term', sorter: 'string', editor: 'input' },
      { title: 'Flags', field: 'flags', sorter: 'string', editor: 'input' },
      { title: 'Status', field: 'status', hozAlign: 'center', formatter: 'tickCross', sorter: 'boolean', editor: true }
    ]
  })

  getTerms(table)
}

function getList (endpoint) {
  const style = $('#vocab').find(':selected').text()
  return $.ajax(API + '/vocab', {
    data: {
      name: style,
      file: endpoint
    },
    dataType: 'json'
  })
}

function annotate (list, type) {
  const terms = []

  let label = 'success'
  if (type === 'reject') {
    label = 'danger'
  }

  for (let i = 0; i < list.length; i++) {
    const entry = list[i].split('/')
    terms.push({
      status: type === 'accept', // `<span class="badge badge-${label}">${type}</span>`,
      term: entry[0],
      flags: entry.length === 1 ? 'N/A' : entry[1]
    })
  }

  return terms
}

function getTerms (table) {
  $.when(getList('accept'), getList('reject')).then(function (
    r1,
    r2
  ) {
    const l1 = annotate(r1[0], 'accept')
    const l2 = annotate(r2[0], 'reject')

    const terms = l1.concat(l2)
    console.log(terms)
    table.setData(terms)
  })
}

$('#vocab').on('change', function () {
  initTable()
})

$(document).ready(function () {
  initTable()
})
