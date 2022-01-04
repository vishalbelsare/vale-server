package action

import (
	"encoding/json"
	"errors"
	"regexp"
	"strconv"
	"strings"
)

// Solution is a potential solution to an alert.
type Solution struct {
	Suggestions []string `json:"suggestions"`
	Error       string   `json:"error"`
}

// An Action represents a possible solution to an Alert.
//
// The possible
type Action struct {
	Name   string   // the name of the action -- e.g, 'replace'
	Params []string // a slice of parameters for the given action
}

// An Alert represents a potential error in prose.
type Alert struct {
	Action      Action // a possible solution
	Check       string // the name of the check
	Description string // why `Message` is meaningful
	Line        int    // the source line
	Link        string // reference material
	Message     string // the output message
	Severity    string // 'suggestion', 'warning', or 'error'
	Span        []int  // the [begin, end] location within a line
	Match       string // the actual matched text
}

type fixer func(Alert) ([]string, error)

var fixers = map[string]fixer{
	"suggest": suggest,
	"replace": replace,
	"remove":  remove,
	"convert": convert,
	"edit":    edit,
}

// ParseAlert returns a slice of suggestions for the given Vale alert.
func ParseAlert(s string) (Solution, error) {
	body := Alert{}
	resp := Solution{}

	err := json.Unmarshal([]byte(s), &body)
	if err != nil {
		return Solution{}, err
	}

	suggestions, err := processAlert(body)
	if err != nil {
		resp.Error = err.Error()
	}
	resp.Suggestions = suggestions

	return resp, nil
}

func processAlert(alert Alert) ([]string, error) {
	action := alert.Action.Name
	if f, found := fixers[action]; found {
		return f(alert)
	}
	return []string{}, errors.New("unknown action")
}

func suggest(alert Alert) ([]string, error) {
	// TODO
	return []string{}, nil
}

func replace(alert Alert) ([]string, error) {
	return alert.Action.Params, nil
}

func remove(alert Alert) ([]string, error) {
	return []string{""}, nil
}

func convert(alert Alert) ([]string, error) {
	match := alert.Match
	if alert.Action.Params[0] == "simple" {
		match = simple(match)
	}
	return []string{match}, nil
}

func edit(alert Alert) ([]string, error) {
	match := alert.Match

	switch name := alert.Action.Params[0]; name {
	case "replace":
		regex, err := regexp.Compile(alert.Action.Params[1])
		if err != nil {
			return []string{}, err
		}
		match = regex.ReplaceAllString(match, alert.Action.Params[2])
	case "trim":
		match = strings.TrimRight(match, alert.Action.Params[1])
	case "remove":
		match = strings.Trim(match, alert.Action.Params[1])
	case "truncate":
		match = strings.Split(match, alert.Action.Params[1])[0]
	case "split":
		index, err := strconv.Atoi(alert.Action.Params[2])
		if err != nil {
			return []string{}, err
		}
		match = strings.Split(match, alert.Action.Params[1])[index]
	}

	return []string{strings.TrimSpace(match)}, nil
}
