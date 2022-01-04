package action

import (
	"archive/zip"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"path/filepath"
	"regexp"
	"strings"
	"unicode"
)

var spaces = regexp.MustCompile(" +")
var stylesPath = "../testdata/styles"

// Response is returned after an action.
type Response struct {
	Msg     string
	Success bool
	Error   string
}

func getJSON(url string) ([]byte, error) {
	resp, err := http.Get(url)
	if err != nil {
		return []byte{}, err
	}
	return ioutil.ReadAll(resp.Body)
}

// Fetch an external (compressed) resource.
//
// For example:
//
// https://www.languagetool.org/download/LanguageTool-4.7.zip
func fetch(src, dst string, debug bool) error {
	// Fetch the resource from the web:
	resp, err := http.Get(src)
	if err != nil {
		return err
	}

	// Create a temp file to represent the archive locally:
	tmpfile, err := ioutil.TempFile("", "temp.*.zip")
	if err != nil {
		return err
	}
	defer os.Remove(tmpfile.Name()) // clean up

	// Write to the  local archive:
	_, err = io.Copy(tmpfile, resp.Body)
	if err != nil {
		return err
	} else if err = tmpfile.Close(); err != nil {
		return err
	}

	return unzip(tmpfile.Name(), dst)
}

func isDir(filename string) bool {
	fi, err := os.Stat(filename)
	return err == nil && fi.IsDir()
}

func fileExists(filename string) bool {
	_, err := os.Stat(filename)
	return err == nil
}

func removeCase(s string, sep string, t func(rune) rune) string {
	out := ""
	old := ' '
	for i, c := range s {
		alpha := unicode.IsLetter(c) || unicode.IsNumber(c)
		mat := (i > 1 && unicode.IsLower(old) && unicode.IsUpper(c))
		if mat || !alpha || (unicode.IsSpace(c) && c != ' ') {
			out += " "
		}
		if alpha || c == ' ' {
			out += string(t(c))
		}
		old = c
	}
	return spaces.ReplaceAllString(strings.TrimSpace(out), sep)
}

func simple(s string) string {
	return removeCase(s, " ", unicode.ToLower)
}

func dash(s string) string {
	return removeCase(s, "-", unicode.ToLower)
}

func snake(s string) string {
	return removeCase(s, "_", unicode.ToLower)
}

func dot(s string) string {
	return removeCase(s, ".", unicode.ToLower)
}

func constant(s string) string {
	return removeCase(s, "_", unicode.ToUpper)
}

func pascal(s string) string {
	out := ""
	wasSpace := false
	for i, c := range removeCase(s, " ", unicode.ToLower) {
		if i == 0 || wasSpace {
			c = unicode.ToUpper(c)
		}
		wasSpace = c == ' '
		if !wasSpace {
			out += string(c)
		}
	}
	return out
}

func camel(s string) string {
	first := ' '
	for _, c := range s {
		if unicode.IsLetter(c) || unicode.IsNumber(c) {
			first = c
			break
		}
	}
	body := pascal(s)
	if len(body) > 1 {
		return strings.TrimSpace(string(unicode.ToLower(first)) + body[1:])
	}
	return s
}

func unzip(src, dest string) error {
	r, err := zip.OpenReader(src)
	if err != nil {
		return err
	}
	defer func() {
		if err := r.Close(); err != nil {
			panic(err)
		}
	}()

	os.MkdirAll(dest, 0755)

	// Closure to address file descriptors issue with all the deferred .Close() methods
	extractAndWriteFile := func(f *zip.File) error {
		rc, err := f.Open()
		if err != nil {
			return err
		}
		defer func() {
			if err := rc.Close(); err != nil {
				panic(err)
			}
		}()

		path := filepath.Join(dest, f.Name)

		// Check for ZipSlip (Directory traversal)
		if !strings.HasPrefix(path, filepath.Clean(dest)+string(os.PathSeparator)) {
			return fmt.Errorf("illegal file path: %s", path)
		}

		if f.FileInfo().IsDir() {
			os.MkdirAll(path, f.Mode())
		} else {
			os.MkdirAll(filepath.Dir(path), f.Mode())
			f, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, f.Mode())
			if err != nil {
				return err
			}
			defer func() {
				if err := f.Close(); err != nil {
					panic(err)
				}
			}()

			_, err = io.Copy(f, rc)
			if err != nil {
				return err
			}
		}
		return nil
	}

	for _, f := range r.File {
		err := extractAndWriteFile(f)
		if err != nil {
			return err
		}
	}

	return nil
}
