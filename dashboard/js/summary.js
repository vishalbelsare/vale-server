var API = "http://localhost:" + location.port;
var reZeroWidthWhite = new RegExp("\u2028|\u2029|\xe2\x80\x8b");
var ruleToCount = {};

var $table = $("#table");
var schema = {
  title: "Thresholds",
  type: "object",
  required: ["Errors", "Warnings", "Suggestions"],
  properties: {
    Errors: {
      type: "object",
      format: "grid",
      properties: {
        count: { type: "integer", default: 0, minimum: -1 },
        scheme: { type: "string", enum: ["strict", "sliding"] },
      },
    },
    Warnings: {
      type: "object",
      options: { collapsed: true },
      format: "grid",
      properties: {
        count: { type: "integer", default: 0, minimum: -1 },
        scheme: { type: "string", enum: ["strict", "sliding"] },
      },
    },
    Suggestions: {
      type: "object",
      options: { collapsed: true },
      format: "grid",
      properties: {
        count: { type: "integer", default: 0, minimum: -1 },
        scheme: { type: "string", enum: ["strict", "sliding"] },
      },
    },
    /*Rules: {
      type: 'array',
      format: 'table',
      title: 'Rules',
      uniqueItems: true,
      items: {
        type: 'object',
        title: 'Rule',
        properties: {
          name: {
            type: 'string',
          },
          count: {
            type: 'integer',
            default: 0,
            minimum: -1,
          },
        },
      },
      default: [
        {
          name: 'Vale.Spelling',
          count: 0,
        },
      ],
    },*/
  },
};

var $editor = new JSONEditor(document.getElementById("json-editor"), {
  schema: schema,
  theme: "bootstrap4",
  iconlib: "fontawesome4",
  disable_edit_json: true,
  disable_properties: true,
});

function initTable() {
  $table.bootstrapTable("destroy").bootstrapTable({
    exportOptions: { ignoreColumn: [0] },
    exportDataType: "all",
    exportTypes: ["json", "csv"],
    locale: "en-US",
    classes: "table table-striped table-hover",
    columns: [
      [
        {
          field: "grade",
          title: "Grade",
          sortable: true,
          formatter: statusFormatter,
          align: "center",
        },
        {
          field: "file",
          title: "File",
          sortable: true,
          formatter: fileFormatter,
          halign: "center",
          align: "left",
        },
        {
          field: "errors",
          title: "Errors",
          sortable: true,
          align: "center",
        },
        {
          field: "warnings",
          title: "Warnings",
          sortable: true,
          align: "center",
        },
        {
          field: "suggestions",
          title: "Suggestions",
          sortable: true,
          align: "center",
        },
      ],
    ],
  });
}

function customSearch(data, text) {
  return data.filter(function (row) {
    return row.file.indexOf(text) > -1;
  });
}

function fileFormatter(value, row, index) {
  return [
    '<a href="javascript:void(0)" onclick="openPath(\'' +
      value.replaceAll(/\\/gi, "\\\\") +
      "')\">" +
      value +
      "</a>",
  ].join("");
}

function openPath(path) {
  $.ajax({
    type: "POST",
    url: API + "/open",
    datatype: "json",
    data: { path: path },
    success: function (data, status, xhr) {
      console.log("Opening", path, data);
    },
    error: function (e) {
      console.log("error", e);
    },
  });
}

function statusFormatter(value, row, index) {
  switch (value) {
    case "A":
      return [
        '<svg height="30" version="1.1" width="30" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" style="overflow: hidden; position: relative; left: -0.015625px; top: -0.546875px;"><desc style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></desc><defs style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></defs><circle cx="50%" cy="50%" r="40%" fill="none" stroke="#20a258" stroke-width="2" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></circle><text x="15" y="15" text-anchor="middle" font-family="&quot;Arial&quot;" font-size="16px" stroke="none" fill="#20a258" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0); text-anchor: middle; font-family: Arial; font-size: 16px;"><tspan dy="5.5" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);">A</tspan></text></svg>',
      ].join("");
    case "B":
      return [
        '<svg height="30" version="1.1" width="30" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" style="overflow: hidden; position: relative; left: -0.015625px; top: -0.546875px;"><desc style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></desc><defs style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></defs><circle cx="50%" cy="50%" r="40%" fill="none" stroke="#66bd63" stroke-width="2" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></circle><text x="15" y="15" text-anchor="middle" font-family="&quot;Arial&quot;" font-size="16px" stroke="none" fill="#66bd63" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0); text-anchor: middle; font-family: Arial; font-size: 16px;"><tspan dy="5.5" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);">B</tspan></text></svg>',
      ].join("");
    case "C":
      return [
        '<svg height="30" version="1.1" width="30" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" style="overflow: hidden; position: relative; left: -0.015625px; top: -0.546875px;"><desc style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></desc><defs style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></defs><circle cx="50%" cy="50%" r="40%" fill="none" stroke="#ffb83f" stroke-width="2" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></circle><text x="15" y="15" text-anchor="middle" font-family="&quot;Arial&quot;" font-size="16px" stroke="none" fill="#ffb83f" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0); text-anchor: middle; font-family: Arial; font-size: 16px;"><tspan dy="5.5" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);">C</tspan></text></svg>',
      ].join("");
    case "D":
      return [
        '<svg height="30" version="1.1" width="30" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" style="overflow: hidden; position: relative; left: -0.015625px; top: -0.546875px;"><desc style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></desc><defs style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></defs><circle cx="50%" cy="50%" r="40%" fill="none" stroke="#fe7418" stroke-width="2" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></circle><text x="15" y="15" text-anchor="middle" font-family="&quot;Arial&quot;" font-size="16px" stroke="none" fill="#fe7418" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0); text-anchor: middle; font-family: Arial; font-size: 16px;"><tspan dy="5.5" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);">D</tspan></text></svg>',
      ].join("");
    default:
      return [
        '<svg height="30" version="1.1" width="30" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" style="overflow: hidden; position: relative; left: -0.015625px; top: -0.546875px;"><desc style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></desc><defs style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></defs><circle cx="50%" cy="50%" r="40%" fill="none" stroke="#d3433b" stroke-width="2" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);"></circle><text x="15" y="15" text-anchor="middle" font-family="&quot;Arial&quot;" font-size="16px" stroke="none" fill="#d3433b" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0); text-anchor: middle; font-family: Arial; font-size: 16px;"><tspan dy="5.5" style="-webkit-tap-highlight-color: rgba(0, 0, 0, 0);">F</tspan></text></svg>',
      ].join("");
  }
}

function toLetterGrade(arr) {
  var textG = "";
  if (arr >= 90) {
    textG = "A";
  } else if (arr >= 80) {
    textG = "B";
  } else if (arr >= 70) {
    textG = "C";
  } else if (arr >= 60) {
    textG = "D";
  } else {
    textG = "F";
  }
  return textG;
}

function calculateScale(total, count) {
  if (total - count >= 0) {
    return (total - count) / total;
  }

  const relative = ((1 - (count - total) / total) * 50) / 100;
  if (relative < 0) {
    return 0;
  }

  return relative;
}

function calculateGrade(e, w, s, rules) {
  var settings = $editor.getValue();

  const et = settings.Errors.count;
  const wt = settings.Warnings.count;
  const st = settings.Suggestions.count;

  if (et > -1 && e > et && settings.Errors.scheme === "strict") {
    return "F";
  } else if (wt > -1 && w > wt && settings.Warnings.scheme === "strict") {
    return "F";
  } else if (st > -1 && s > st && settings.Suggestions.scheme === "strict") {
    return "F";
  }

  /*for (var i = 0; i < settings.Rules.length; i++) {
    var rule = settings.Rules[i];
    if (rules[rule.name] > rule.count) {
      console.log('Hmm', rules[rule.name], rule.count);
      return 'F';
    }
  }*/

  const es = calculateScale(et, e);
  const ws = calculateScale(wt, w);
  const ss = calculateScale(st, s);

  var intGrade = (es + ws + ss) / 3;
  return toLetterGrade(intGrade * 100);
}

function summarize(obj) {
  // Global ...
  ruleToCount = {};

  var rows = [];
  for (const key in obj) {
    ruleToCount[key] = {};
    const alerts = obj[key];

    var e = 0,
      s = 0,
      w = 0;
    for (var i = 0; i < alerts.length; ++i) {
      const level = alerts[i].Severity;
      if (level === "error") {
        e += 1;
      } else if (level === "warning") {
        w += 1;
      } else {
        s += 1;
      }

      var check = alerts[i].Check;
      if (check in ruleToCount[key]) {
        ruleToCount[key][check]++;
      } else {
        ruleToCount[key][check] = 1;
      }
    }

    rows.push({
      grade: calculateGrade(e, w, s, ruleToCount),
      file: key,
      errors: e,
      warnings: w,
      suggestions: s,
    });
  }

  $table.bootstrapTable("load", rows);
  showBreakdown(e, w, s, false, false);
}

function getSummary(source) {
  showBreakdown(0, 0, 0, true, false);
  $table.bootstrapTable("removeAll");
  $.ajax({
    type: "POST",
    url: API + "/file",
    datatype: "json",
    data: { file: source, raw: true, path: source },
    success: function (data, status, xhr) {
      if ("error" in data && data.error === "timeout") {
        showBreakdown(0, 0, 0, false, true);
        $("#summary-error").removeClass("collapse").addClass("show");
      } else {
        summarize(data);
      }
    },
    error: function (jqXHR, exception) {
      showBreakdown(0, 0, 0, false, true);
      console.log("error", exception);
    },
  });
}

function showBreakdown(errors, warnings, suggestions, loading, idle) {
  $("#breakdown").empty();
  if (loading) {
    $(
      '<div class="progress-bar progress-bar-striped progress-bar-animated" role="progressbar" style="width: 100%;" aria-valuenow="100" aria-valuemin="0" aria-valuemax="100" />',
      {}
    ).appendTo("#breakdown");
  } else if (idle) {
    $(
      '<div class="progress-bar bg-info" role="progressbar" style="width: 100%;" aria-valuenow="100" aria-valuemin="0" aria-valuemax="100" />',
      {}
    ).appendTo("#breakdown");
  } else {
    const total = errors + warnings + suggestions;

    const sp = (suggestions / total) * 100;
    const wp = (warnings / total) * 100;
    const ep = (errors / total) * 100;

    $(
      '<div class="progress-bar bg-info" role="progressbar" style="width:' +
        sp.toString() +
        '%" aria-valuenow="' +
        sp.toString() +
        '" aria-valuemin="0" aria-valuemax="100"></div>',
      {}
    ).appendTo("#breakdown");
    $(
      '<div class="progress-bar bg-warning" role="progressbar" style="width:' +
        wp.toString() +
        '%" aria-valuenow="' +
        wp.toString() +
        '" aria-valuemin="0" aria-valuemax="100"></div>',
      {}
    ).appendTo("#breakdown");
    $(
      '<div class="progress-bar bg-danger" role="progressbar" style="width:' +
        ep.toString() +
        '%" aria-valuenow="' +
        ep.toString() +
        '" aria-valuemin="0" aria-valuemax="100"></div>',
      {}
    ).appendTo("#breakdown");
  }
}

function detailFormatter(index, row) {
  var html = [];
  $.each(row, function (key, value) {
    if (key === "file") {
      var obj = ruleToCount[value];
      /*var max = Object.keys(obj).reduce(function(a, b) {
        return obj[a] > obj[b] ? a : b
      });*/
      html.push("<pre><code>" + JSON.stringify(obj, null, 2) + "</code></pre>");
    }
  });
  return html.join("");
}

$(function () {
  initTable();

  $("#generate").click(function () {
    getSummary($("#source").val());
  });

  $.getJSON(API + "/report", function (data) {
    $editor.setValue(data);
  });

  const queryString = window.location.search;
  const urlParams = new URLSearchParams(queryString);
  const path = urlParams.get("path");
  if (path) {
    $("#source").val(path);
    getSummary(path);
  }

  $("#save-json").click(function (e) {
    e.preventDefault();

    const errors = $editor.validate();
    if (errors.length) {
      console.log(errors);
    } else {
      const config = JSON.stringify($editor.getValue());
      $.ajax({
        type: "POST",
        url: API + "/save",
        datatype: "json",
        data: { report: config },
        success: function (data, status, xhr) {
          console.log(data);
        },
        error: function (e) {
          console.log("error", e);
        },
      });
      $("#task-add-modal").modal("hide");
    }
  });
});
