package action

import (
	"bytes"
	"encoding/json"
	"io/ioutil"
	"net/http"
)

func Upload(regex, path string) (string, error) {
	text, err := ioutil.ReadFile(path)
	if err != nil {
		return "", err
	}

	// TODO: What should the flavor be for existance?
	payload, err := json.Marshal(map[string]string{
		"regex":      regex,
		"testString": string(text),
		"flags":      "gm",
		"delimiter":  "`",
		"flavor":     "golang",
	})

	if err != nil {
		return "", err
	}

	resp, err := http.Post(
		"https://regex101.com/api/regex", "application/json",
		bytes.NewBuffer(payload))

	if err != nil {
		return "", err
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return "", err
	}

	return string(body), nil
}
