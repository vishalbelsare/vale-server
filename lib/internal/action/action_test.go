package action

import (
	"testing"
)

func TestConvert(t *testing.T) {
	cases := map[string]string{
		"auto-sorting": "auto sorting",
	}
	for k, v := range cases {
		alert := Alert{
			Match:  k,
			Action: Action{Name: "convert", Params: []string{"simple"}},
		}
		suggestions, err := convert(alert)
		if err != nil {
			t.Error(err)
		} else if suggestions[0] != v {
			t.Errorf("Bad conversion for '%v', %v", k, suggestions)
		}
	}
}

func TestSuggest(t *testing.T) {
	cases := map[string]string{
		"acceptible": "acceptable",
		"accomodate": "accommodate",
		"Accomodate": "Accommodate",
	}
	for k, v := range cases {
		alert := Alert{
			Match:  k,
			Action: Action{Name: "suggest", Params: []string{"spellings"}},
		}
		suggestions, err := suggest(alert)
		if err != nil {
			t.Error(err)
		} else if !stringInSlice(v, suggestions) {
			t.Errorf("Bad suggestions for '%v', %v", k, suggestions)
		}
	}
}
