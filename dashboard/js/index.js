var API = "http://localhost:" + location.port;

function versionModel() {
    var self = this;

    self.version = ko.observable("");
    $.getJSON(API + "/version", function (data) {
        self.version("v" + data.server);
    });
}

function switchTheme(toDark) {
    if (toDark) {
        $('#main-nav').removeClass('navbar-light')
        $('#main-nav').removeClass('bg-white')
        document.body.setAttribute('data-theme', 'dark')
        $('#main-nav').addClass('navbar-dark')
        $('#main-nav').addClass('bg-dark')
        $('#env').addClass('bg-dark')
        $('#env-table').addClass('text-light')
        localStorage.setItem('darkSwitch', 'dark')
    } else {
        $('#main-nav').removeClass('navbar-dark')
        $('#main-nav').removeClass('bg-dark')
        document.body.removeAttribute('data-theme')
        $('#main-nav').addClass('navbar-light')
        $('#main-nav').addClass('bg-white')
        $('#env').removeClass('bg-dark')
        $('#env-table').removeClass('text-light')
        localStorage.removeItem('darkSwitch')
    }
}

$(document).ready(function () {
    var vm = new versionModel();
    ko.applyBindings(vm, document.getElementById("vbadge"));

    $.getJSON(API + "/system", function (data) {
        $('#env-table tbody').after(
            "<tr>" + `<th><i class="fa fa-server" aria-hidden="true"></i> Vale Server</th>` +
            `<td class="text-right text-success">${data["server"]}, ${data["platform"]}</td>` +
            "</tr>");
        for (const key in data) {
            // Skip info used above
            if (key === "platform" || key === "server") {
                continue
            }

            // Truncate some long values
            if (key === "asciidoctor") {
                data[key] = data[key].replace(/\[https:\/\/asciidoctor\.org\]|\(lc:.*\)/g, "").trim()
            } else if (key === "xsltproc") {
                data[key] = data[key].split(/\r?\n/)[0]
            }

            // Handle missing values
            if (data[key] === "") {
                $('#env-table tr:last').after(
                    "<tr>" +
                    `<th><i class="fa fa-terminal" aria-hidden="true"></i> <code>${key}</code></th>` +
                    `<td class="text-right text-danger">not found</td></tr>`);
            } else {
                $('#env-table tr:last').after(
                    "<tr>" +
                    `<th><i class="fa fa-terminal" aria-hidden="true"></i> <code>${key}</code></th>` +
                    `<td class="text-right text-success">${data[key]}</td>` +
                    "</tr>");
            }
        }
    });

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
            switchTheme(darkThemeSelected)
        }

        function resetTheme() {
            switchTheme(darkSwitch.checked)
        }
    }
})
