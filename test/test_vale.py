import itertools
import json

from . import query, json_equal, FIXTURES


def test_suggest():
    """Check the `/suggest` endpoint on various alerts."""
    documents = FIXTURES / "json"
    for case in [c for c in documents.iterdir() if c.is_dir()]:
        expected = case / "out.json"
        assert expected.exists(), "Output file exists"
        with expected.open() as fp:
            output = json.load(fp)

        doc = case / "test.json"
        assert doc.exists(), "JSON file exists"

        r = query("suggest", {"alert": doc.read_text()})
        assert r.json() == output


def test_vale():
    """Check the `/vale` endpoint on documents of varying sizes and formats."""
    documents = FIXTURES / "documents"
    for case in [c for c in documents.iterdir() if c.is_dir()]:
        expected = case / "expected.json"
        assert expected.exists(), "Output file exists"
        with expected.open(encoding="utf-8") as fp:
            output = json.load(fp)

        config = case / ".vale.ini"
        assert config.exists(), "Config file exists"

        docs = [case.glob(e) for e in ["*.md", "*.rst", "*.adoc"]]
        doc = list(itertools.chain(*docs))[0]
        assert doc.exists(), "Document file exists"

        r = query(
            "vale",
            {
                "text": doc.read_text(encoding="utf-8"),
                "format": doc.suffix,
                "config": config.read_text(),
            },
        )

        assert json_equal(r.json(), output) == True
