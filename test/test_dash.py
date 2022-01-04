import os
import shutil

import pytest

from . import query, json_equal, action, FIXTURES, SERVER, STYLES_PATH


@pytest.fixture(scope="module")
def browser():
    from splinter import Browser

    # Defaults to Firefox.
    #
    # TODO: More browsers?
    browser = Browser()

    yield browser

    # Tear down ...
    browser.quit()


def test_load(browser):
    """Check the basic structure of the Dashboard's homepage."""
    # Open the Dashboard
    browser.visit(SERVER)

    # Check the title:
    heading = browser.find_by_tag("h1").first
    assert heading.text.startswith("Vale Dashboard") == True


def test_install(browser):
    """Install and remove an external style."""
    style = os.path.join(STYLES_PATH, "write-good")

    if os.path.exists(style):
        shutil.rmtree(style)
    assert os.path.exists(style) == False

    browser.visit(SERVER)

    # Navigate to the Styles page:
    action(lambda: browser.click_link_by_id("styles-page"))

    # Check the title:
    heading = browser.find_by_tag("h1").first
    assert heading.text == "Styles"

    browser.find_by_id("style-filter").first.fill("write-")

    action(lambda: browser.find_by_id("style-filter").first.fill("write-"))

    action(lambda: browser.find_by_text("Install").first.click(), delay=5)

    assert os.path.exists(style) == True


def test_add_term(browser):
    """Install and remove an external style."""
    # TODO: Support switching "active" project!
    style = os.path.join(STYLES_PATH, "Vocab", "Test", "reject.txt")
    assert os.path.exists(style) == True

    text = "OS X is an operating system."
    r = query("vale", {"text": text, "format": ".md"})
    assert r.json() == {}

    browser.visit(SERVER)

    # Navigate to the Vocab page:
    action(lambda: browser.click_link_by_id("vocab-page"))

    # Check the title:
    heading = browser.find_by_tag("h1").first
    assert heading.text == "Vocab"

    # Add the term:
    action(lambda: browser.find_by_id("add").first.click())

    action(lambda: browser.find_by_id("termName").first.fill("OS X"), delay=0)
    action(lambda: browser.find_by_id("sel2").first.select("2"))

    action(lambda: browser.find_by_id("open-term").first.click(), delay=0)

    r = query("vale", {"text": text, "format": ".md"})
    assert json_equal(
        r.json(),
        {
            "test.md": [
                {
                    "Action": {"Name": "", "Params": None},
                    "Check": "Vale.Avoid",
                    "Description": "",
                    "Line": 1,
                    "Link": "",
                    "Match": "OS X",
                    "Message": "Avoid using 'OS X'.",
                    "Severity": "error",
                    "Span": [1, 4],
                }
            ]
        },
    )

    action(lambda: browser.check("btSelectAll"))
    action(lambda: browser.find_by_id("remove").first.click(), delay=0)

    r = query("vale", {"text": text, "format": ".md"})
    assert r.json() == {}
