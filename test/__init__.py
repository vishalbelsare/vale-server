import os
import platform
import time

import requests

from pathlib import Path
from urllib import parse as urlprase


SERVER = "http://localhost:7777"
FIXTURES = Path("fixtures")

if platform.system() == "Windows":
    STYLES_PATH = "C:\\Users\\jdkato\\AppData\\Roaming\\errata-ai\\Vale Server\\styles"
else:
    support = "/Users/jdkato/Library/Application Support"
    STYLES_PATH = os.path.join(support, "errata-ai/Vale Server/styles")


def query(endpoint, payload, method="post"):
    """query the given Vale Server endpoint and return its response.
    """
    target = urlprase.urljoin(SERVER, endpoint)
    return getattr(requests, method)(target, payload)


def ordered(obj):
    """Order a dictionary.

    See https://bit.ly/2Ml1Afj.
    """
    if isinstance(obj, dict):
        return sorted((k, ordered(v)) for k, v in obj.items())
    if isinstance(obj, list):
        return sorted(ordered(x) for x in obj)
    else:
        return obj


def json_equal(o1, o2):
    """Compare two JSON objects.
    """
    observed = {}
    # NOTE: Since Vale Server uses temp files, we have to manually
    # convert the filename to 'test.md':
    observed["test.md"] = o1[list(o1.keys())[0]]
    return ordered(observed) == ordered(o2)


def action(action, delay=3):
    """Perform a browser action followed by a specified delay.
    """
    action()
    time.sleep(delay)
