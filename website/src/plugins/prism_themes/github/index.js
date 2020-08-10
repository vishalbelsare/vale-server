var theme = {
        "plain": {
            "color": "#fff",
            "backgroundColor": "#1e1e1e"
        },
        "styles": [{
            "types": ["comment", "prolog", "doctype", "cdata"],
            "style": {
                "color": "rgb(161, 155, 143)",
                "fontStyle": "italic"
            }
        }, {
            "types": ["namespace"],
            "style": {
                "opacity": 0.7
            }
        }, {
            "types": ["string", "attr-value"],
            "style": {
                "color": "rgb(33, 175, 144)"
            }
        }, {
            "types": ["punctuation", "operator"],
            "style": {
                "color": "#393A34"
            }
        }, {
            "types": ["entity", "url", "symbol", "number", "boolean", "variable", "constant", "property", "regex", "inserted"],
            "style": {
                "color": "rgb(102, 181, 246)"
            }
        }, {
            "types": ["atrule", "keyword", "attr-name", "selector"],
            "style": {
                "color": "rgb(243, 153, 97)"
            }
        }, {
            "types": ["function", "deleted", "tag"],
            "style": {
                "color": "rgb(187, 151, 231)"
            }
        }, {
            "types": ["function-variable"],
            "style": {
                "color": "#6f42c1"
            }
        }, {
            "types": ["tag", "selector", "keyword"],
            "style": {
                "color": "rgb(243, 153, 97)"
            }
        }]
}

module.exports = theme;
