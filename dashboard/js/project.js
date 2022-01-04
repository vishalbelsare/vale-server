var API = "http://localhost:" + location.port;

function projectModel() {
    var self = this;

    self.project = ko.observable("");
    $.getJSON(API + "/project", function (data) {
        self.project(data.project);
    });
}

$(function () {
    var pvm = new projectModel();

    ko.applyBindings(pvm, document.getElementById("project"));
    $(window).focus(function () {
        $.getJSON(API + "/project", function (data) {
            if (data.project !== pvm.project()) {
                location.reload();
            }
        });
    });
});
