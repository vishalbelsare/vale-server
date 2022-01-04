package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"os/exec"

	"valelib/internal/action"
)

// version is set during the release build process.
var version = "master"

// Response is returned after an action.
type Response struct {
	Msg     string
	Success bool
	Error   string
}

func printJSON(t interface{}) error {
	b, err := json.MarshalIndent(t, "", "  ")
	if err != nil {
		fmt.Println("{}")
		return err
	}
	fmt.Println(string(b))
	return nil
}

// Send a JSON response after a local action.
func sendResponse(msg string, err error) error {
	resp := Response{Msg: msg, Success: err == nil}
	if !resp.Success {
		resp.Error = err.Error()
	}
	return printJSON(resp)
}

func run() error {
	args := os.Args[2:]
	switch os.Args[1] {
	case "suggest":
		resp, err := action.ParseAlert(args[0])
		if err != nil {
			return sendResponse("failed to process alert", err)
		}
		return printJSON(resp)
	case "new-project":
		if len(args) != 2 {
			return sendResponse(
				"failed to create project",
				errors.New("invalid arguments; expected 2"),
			)
		}
		return action.AddProject(args[0], args[1])
	case "remove-project":
		if len(args) != 2 {
			return sendResponse(
				"failed to create project",
				errors.New("invalid arguments; expected 2"),
			)
		}
		return action.RemoveProject(args[0], args[1])
	case "edit-project":
		return action.EditProject(args)
	case "ls-projects":
		projects, err := action.ListDir(args[0], "Vocab")
		if err != nil {
			return sendResponse("failed to list projects", err)
		}
		return printJSON(projects)
	case "get-vocab":
		words, err := action.GetVocab(args)
		if err != nil {
			return sendResponse("failed to get vocab", err)
		}
		return printJSON(words)
	case "update-vocab":
		err := action.UpdateVocab(args)
		if err != nil {
			return sendResponse(
				fmt.Sprintf("Failed to update '%s'", args[1]),
				err)
		}
		return sendResponse(fmt.Sprintf(
			"Successfully updated '%s'", args[1]), nil)
	case "ls-styles":
		styles, err := action.ListDir(args[0], "")
		if err != nil {
			return sendResponse("failed to list styles", err)
		}
		return printJSON(styles)
	case "ls-library":
		styles, err := action.GetLibrary(args[0])
		if err != nil {
			return sendResponse("Failed to fetch library", err)
		}
		return printJSON(styles)
	case "install":
		err := action.InstallStyle(args)
		if err != nil {
			return sendResponse(
				fmt.Sprintf("Failed to install '%s'", args[1]),
				err)
		}
		return sendResponse(fmt.Sprintf(
			"Successfully installed '%s'", args[1]), nil)
	case "get-vale":
		path, err := exec.LookPath("vale")
		if err != nil {
			return sendResponse("failed to find vale", err)
		}
		fmt.Print(path)
		return nil
	case "upload":
		resp, err := action.Upload(args[0], args[1])
		if err != nil {
			return sendResponse("failed to find vale", err)
		}
		fmt.Print(resp)
		return nil
	}
	return errors.New("end of switch")
}

func main() {
	if run() != nil {
		os.Exit(1)
	} else {
		os.Exit(0)
	}
}
