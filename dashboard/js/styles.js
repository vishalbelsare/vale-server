var API = "http://localhost:" + location.port;

function stylesModel(data, access, path) {
  var self = this;

  // import starting data
  self.styles = data;

  // Write-access
  self.hasAccess = ko.observable(access);

  self.path = path;

  // currently selected category
  self.selectedCategory = ko.observable("All");
  self.query = ko.observable("");
  self.state = ko.observable();

  // filtered list
  self.filteredStyles = ko.computed(function () {
    var category = self.selectedCategory();
    var search = self.query();
    var state = self.state();
    var qsRegex = new RegExp(search, "gi");

    var tempList = self.styles.slice();
    return tempList.filter(function (style) {
      // Check title and description
      var matches = search
        ? style.name.match(qsRegex) || style.description.match(qsRegex)
        : true;
      if (category === "Installed") {
        return style.installed && matches;
      } else if (category == "Outdated") {
        return style.has_update && matches;
      }
      return matches;
    });
  });

  self.installStyle = function (style, event) {
    var $btn = $(event.target);
    var $old = $btn.clone();

    $btn.prop("disabled", true);
    $btn.html(
      `<span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>`
    );

    var url = style.url;
    if (style.addon) {
      url = "fetch";
    }

    $.ajax({
      type: "POST",
      url: API + "/install",
      datatype: "json",
      data: { name: style.name, link: url },
      success: function (data, status, xhr) {
        $.getJSON(API + "/library", function (data) {
          self.styles = data;
          self.state.notifySubscribers();
          console.log("state", self.styles);
        });
        if (data.Success === false) {
          $alert = $("#error-alert");
          $alert.find("#error").text(data.Error);
          $alert
            .find("#support")
            .html(
              'Please contact support at <a href="mailto:support@errata.ai">support@errata.ai</a> for assistance.'
            );
          $alert.show();
        } else if (style.addon) {
          $.getJSON(API + "/addons", function (data) {
            console.log("addons", data);
          });
        }
      },
      error: function (jqXHR, exception) {
        $(event.target).replaceWith($old);
        console.log("error", exception);
      },
    });
  };
}

ko.bindingHandlers.bsChecked = {
  init: function (
    element,
    valueAccessor,
    allBindingsAccessor,
    viewModel,
    bindingContext
  ) {
    var value = valueAccessor();
    var newValueAccessor = function () {
      return {
        change: function () {
          value(element.value);
        },
      };
    };
    ko.bindingHandlers.event.init(
      element,
      newValueAccessor,
      allBindingsAccessor,
      viewModel,
      bindingContext
    );
  },
  update: function (
    element,
    valueAccessor,
    allBindingsAccessor,
    viewModel,
    bindingContext
  ) {
    if ($(element).val() == ko.unwrap(valueAccessor())) {
      setTimeout(function () {
        $(element).closest(".btn").button("toggle");
      }, 1);
    }
  },
};

$(function () {
  var $el = document.getElementById("styles-content");
  $.getJSON(API + "/path", function (data) {
    if (data.writable !== "no") {
      $("#styles-wrapper").removeClass("hide");
      $.getJSON(API + "/library", function (library) {
        ko.applyBindings(new stylesModel(library, true, ""), $el);
      });
    } else {
      $("#access-wrapper").removeClass("hide");
      ko.applyBindings(new stylesModel([], false, data.path), $el);
    }
  });
});
