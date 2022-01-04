package action

import (
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"
)

func TestUpdateVocab(t *testing.T) {
	old := []string{"Vale", "Golang"}
	new := []string{"Vale", "Golang", "valelib"}

	vocab, err := GetVocab([]string{stylesPath, "Docs", "accept"})
	if err != nil {
		t.Error(err)
	} else if !reflect.DeepEqual(vocab, old) {
		t.Errorf("Wrong vocab: %v", vocab)
	}

	UpdateVocab([]string{
		stylesPath,
		"Docs.accept",
		strings.Join(new, "\n")})

	vocab, err = GetVocab([]string{stylesPath, "Docs", "accept"})
	if err != nil {
		t.Error(err)
	} else if !reflect.DeepEqual(vocab, new) {
		t.Errorf("Wrong vocab after update: %v", vocab)
	}

	UpdateVocab([]string{
		stylesPath,
		"Docs.accept",
		strings.Join(old, "\n")})

	vocab, err = GetVocab([]string{stylesPath, "Docs", "accept"})
	if err != nil {
		t.Error(err)
	} else if !reflect.DeepEqual(vocab, old) {
		t.Errorf("Wrong vocab after reset: %v", vocab)
	}
}

func TestAddProject(t *testing.T) {
	project := filepath.Join(stylesPath, "Vocab", "Test")
	if isDir(project) {
		t.Error("Project already exists!")
	}

	err := AddProject(stylesPath, "Test")
	if err != nil {
		t.Error(err)
	} else if !isDir(project) {
		t.Error("Project was not created!")
	}

	EditProject([]string{stylesPath, "Test", "RenamedTest"})
	if isDir(project) {
		t.Error("Project was not re-named!")
	}

	project = filepath.Join(stylesPath, "Vocab", "RenamedTest")
	if !isDir(project) {
		t.Error("Project was not re-created!")
	}

	RemoveProject(stylesPath, "RenamedTest")
	if isDir(project) {
		t.Error("Project was not deleted!")
	}
}

func TestGetProjects(t *testing.T) {
	projects, err := ListDir(stylesPath, "Vocab")
	if err != nil {
		t.Error(err)
	} else if !reflect.DeepEqual(projects, []string{"Docs", "Marketing", "sales"}) {
		t.Errorf("Wrong projects: %v", projects)
	}
}

func TestInstallStyle(t *testing.T) {
	style := filepath.Join(stylesPath, "write-good")

	args := []string{
		stylesPath,
		"write-good",
		"https://github.com/errata-ai/write-good/releases/latest/download/write-good.zip"}

	err := InstallStyle(args)
	if err != nil {
		t.Error(err)
	} else if !isDir(style) {
		t.Error("write-good is not installed!")
	}

	os.RemoveAll(style)
}

func TestGetLibrary(t *testing.T) {
	styles, err := GetLibrary(stylesPath)
	if err != nil {
		t.Error(err)
	}

	for _, style := range styles {
		if style.Name == "Google" {
			if style.InLibrary != true {
				t.Error("Google should be in library!")
			} else if style.Installed != true {
				t.Error("Google should be in path!")
			}
		}
	}
}
