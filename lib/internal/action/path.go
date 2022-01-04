package action

import (
	"bufio"
	"encoding/json"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"

	"valelib/internal/atom"
)

var library = "https://raw.githubusercontent.com/errata-ai/styles/master/library.json"

// Style represents an externally-hosted style.
type Style struct {
	// User-provided fields.
	Author      string `json:"author"`
	Description string `json:"description"`
	Deps        string `json:"deps"`
	Feed        string `json:"feed"`
	Homepage    string `json:"homepage"`
	Name        string `json:"name"`
	URL         string `json:"url"`

	// Generated fields.
	HasUpdate bool `json:"has_update"`
	InLibrary bool `json:"in_library"`
	Installed bool `json:"installed"`
	Addon     bool `json:"addon"`
}

// Meta represents an installed style's meta data.
type Meta struct {
	Author      string   `json:"author"`
	Coverage    float64  `json:"coverage"`
	Description string   `json:"description"`
	Email       string   `json:"email"`
	Feed        string   `json:"feed"`
	Lang        string   `json:"lang"`
	License     string   `json:"license"`
	Name        string   `json:"name"`
	Sources     []string `json:"sources"`
	URL         string   `json:"url"`
	Vale        string   `json:"vale_version"`
	Version     string   `json:"version"`
}

// GetLibrary returns the latest style library.
func GetLibrary(path string) ([]Style, error) {
	styles := []Style{}

	resp, err := getJSON(library)
	if err != nil {
		return styles, err
	} else if err = json.Unmarshal(resp, &styles); err != nil {
		return styles, err
	}

	for i := 0; i < len(styles); i++ {
		s := &styles[i]
		s.InLibrary = true
	}

	err = filepath.WalkDir(path, func(fp string, de os.DirEntry, err error) error {
		if strings.Contains(fp, "add-ons") {
			return filepath.SkipDir
		} else if de.Name() != "meta.json" {
			return nil
		}
		name := filepath.Base(filepath.Dir(fp))
		meta := Meta{}

		f, _ := ioutil.ReadFile(fp)
		if err := json.Unmarshal(f, &meta); err != nil {
			return err
		}

		style := &Style{}
		for i := 0; i < len(styles); i++ {
			s := &styles[i]
			if s.Name == name {
				s.Installed = true
				style = s
				break
			}
		}

		feed, err := atom.Parse(meta.Feed)
		if err != nil {
			return err
		}

		t, err := atom.ToTime(feed.Updated)
		if err != nil {
			return err
		}

		info, err := os.Stat(fp)
		if err != nil {
			return err
		} else if info.ModTime().UTC().Before(t.UTC()) {
			style.HasUpdate = true
		}
		return nil
	})

	return styles, err
}

// InstallStyle installs the given style using its download link.
func InstallStyle(args []string) error {
	style := filepath.Join(args[0], args[1])
	if isDir(style) {
		os.RemoveAll(style) // Remove existing version
	}
	return fetch(args[2], args[0], false)
}

// ListDir prints all folders on the current StylesPath.
func ListDir(path, entry string) ([]string, error) {
	var styles []string

	files, err := ioutil.ReadDir(filepath.Join(path, entry))
	if err != nil {
		return styles, err
	}

	for _, f := range files {
		if f.IsDir() && f.Name() != "Vocab" {
			styles = append(styles, f.Name())
		}
	}
	return styles, nil
}

// AddProject adds a new project to the current StylesPath.
func AddProject(path, name string) error {
	var ferr error

	root := filepath.Join(path, "Vocab", name)
	if _, err := os.Stat(root); os.IsNotExist(err) {
		os.MkdirAll(root, os.ModePerm)
	}

	for _, f := range []string{"accept.txt", "reject.txt"} {
		ferr = ioutil.WriteFile(filepath.Join(root, f), []byte{}, 0644)
	}

	return ferr
}

// RemoveProject deletes a project from the current StylesPath.
func RemoveProject(path, name string) error {
	return os.RemoveAll(filepath.Join(path, "Vocab", name))
}

// EditProject renames a project from the current StylesPath.
func EditProject(args []string) error {
	old := filepath.Join(args[0], "Vocab", args[1])
	new := filepath.Join(args[0], "Vocab", args[2])
	return os.Rename(old, new)
}

// UpdateVocab updates a vocab file for the given project.
func UpdateVocab(args []string) error {
	parts := strings.Split(args[1], ".")
	dest := filepath.Join(args[0], "Vocab", parts[0], parts[1]+".txt")
	return ioutil.WriteFile(dest, []byte(args[2]), 0644)
}

// GetVocab extracts a vocab file for a project.
func GetVocab(args []string) ([]string, error) {
	vocab := filepath.Join(args[0], "Vocab", args[1], args[2]+".txt")
	words := []string{}

	if fileExists(vocab) {
		f, err := os.Open(vocab)
		if err != nil {
			return words, err
		}
		scanner := bufio.NewScanner(f)
		for scanner.Scan() {
			words = append(words, scanner.Text())
		}
	}

	return words, nil
}
