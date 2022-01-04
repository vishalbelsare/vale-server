var API = "http://localhost:" + location.port;

function switchTheme(toDark) {
    if (toDark) {
        $('#main-nav').removeClass('navbar-light')
        $('#main-nav').removeClass('bg-white')
        document.body.setAttribute('data-theme', 'dark')
        $('#main-nav').addClass('navbar-dark')
        $('#main-nav').addClass('bg-dark')
        $('#header').addClass('bg-dark')
        $('body').addClass('text-light')
        $('#table').addClass('text-light')
        localStorage.setItem('darkSwitch', 'dark')
    } else {
        $('#main-nav').removeClass('navbar-dark')
        $('#main-nav').removeClass('bg-dark')
        document.body.removeAttribute('data-theme')
        $('#main-nav').addClass('navbar-light')
        $('#main-nav').addClass('bg-white')
        $('#header').removeClass('bg-dark')
        $('body').removeClass('text-light')
        $('#table').removeClass('text-light')
        localStorage.removeItem('darkSwitch')
    }
}

function dashModel(access, path) {
    var self = this;

    // Write-access
    self.hasAccess = ko.observable(access);
    self.path = path;
    self.vocab = ko.observableArray([]);

    self.update = function (data) {
        if (data == null || data.length === 0) {
            $("#no-vocab").removeClass("collapse").addClass("show");
            $("#add").prop("disabled", true);
        } else {
            $("#no-vocab").addClass("collapse").removeClass("show");
            $("#add").prop("disabled", false);
        }
        self.vocab(data);
    };

    $.getJSON(API + "/projects", function (data) {
        if (data === null || ("Success" in data && data.Success === false)) {
            self.update([]);
        } else {
            self.update(data);
        }
    });
}

$(document).ready(function () {
    var $el = document.getElementById("vocab-content");
    $.getJSON(API + "/path", function (data) {
        if (data.writable !== "no") {
            $("#vocab-wrapper").removeClass("hide");
            var vm = new dashModel(true, "");

            ko.applyBindings(vm, $el);
            $("#form2").on("submit", function (e) {
                e.preventDefault();
                $.ajax({
                    type: "GET",
                    url: API + "/newVocab",
                    datatype: "json",
                    data: {
                        name: $("#vocabName").val()
                    },
                    success: function (data, status, xhr) {
                        $.getJSON(API + "/projects", function (data) {
                            vm.update(data);
                        });
                        console.log("Created vocab", data);
                    },
                    error: function (error) {
                        console.log("Failed to create vocab", error);
                    },
                });

                $("#newVocabModal").modal("toggle");
                $("#form2").trigger("reset");
            });
        } else {
            $("#access-wrapper").removeClass("hide");
            ko.applyBindings(new dashModel(false, data.path), $el);
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
});
