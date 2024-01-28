// Package main provides a simple text editor that allows basic text manipulation operations.
package main

import (
	"bufio"   //To read input from stdin
	"fmt"     //For stdout operations
	"os"      //To get stdin
	"strconv" //For Atoi
	"strings" //For string methods
)

// Editor represents the state of the text editor.
type Editor struct {
	text           string   // Current text content
	selected       string   // Selected text
	start_selected int      // Start index of selected text
	end_selected   int      // End index of selected text
	cursor         int      // Cursor position
	clipboard      []string // Clipboard for copied text
}

// insertText inserts new_text into the editor's text at the current cursor position.
func insertText(text string, editor *Editor) {
	//If text is selected, remove it and move cursor
	if len(editor.selected) != 0 {
		editor.text = editor.text[:editor.start_selected] + editor.text[editor.end_selected+1:]
		editor.cursor = editor.start_selected

	}
	//Insert text on the cursor position
	start := editor.text[:editor.cursor]
	end := editor.text[editor.cursor:]
	editor.text = start + text + end

	//Move cursor to the end of typped text
	editor.cursor += len(text)
}

// selectArea selects a portion of the editor's text specified by indices.
func selectArea(indices string, editor *Editor) {
	//Get selection indices
	indices_splitted := strings.Split(indices, " ")
	start_index, _ := strconv.Atoi(indices_splitted[0])
	end_index, _ := strconv.Atoi(indices_splitted[1])

	//Select text between indices and store indices
	editor.selected = editor.text[start_index : end_index+1]
	editor.start_selected = start_index
	editor.end_selected = end_index

	//Move cursor to the end of selected area
	editor.cursor = end_index + 1
}

// moveCursor moves the cursor by the specified offset.
func moveCursor(offset_string string, editor *Editor) {
	//Get offset and increment/decrement from cursor
	offset, _ := strconv.Atoi(offset_string)
	editor.cursor += offset

	//Check if new cursor is valid 
	if editor.cursor < 0 {
		editor.cursor = 0
	}
	if editor.cursor > len(editor.text) {
		editor.cursor = len(editor.text)
	}

	//Selected are is cleared
	editor.selected = ""

}

// copy copies the selected text to the clipboard.
func copy(editor *Editor) {
	//If some text is selected, copy it to clipboard
	if len(editor.selected) != 0 {
		editor.clipboard = append(editor.clipboard, editor.selected)
	}
}

// paste pastes text from the clipboard into the editor's text at the current cursor position.
func paste(paste_command []string, editor *Editor) {
	//If steps are provided, get steps othervise 1
	steps_back := 1
	if len(paste_command) != 1 {
		steps_back, _ = strconv.Atoi(paste_command[1])
	}

	//If step number exists in clipboard, paste text
	if steps_back <= len(editor.clipboard) {
		insertText(editor.clipboard[len(editor.clipboard)-steps_back], editor)
	}

}

// executeOption executes the specified command on the editor.
func executeOption(command string, editor *Editor) bool {
	//Get operation type and parameters
	command_splitted := strings.SplitN(command, " ", 2)
	operation := command_splitted[0]

	//Execute the operation
	switch operation {
	case "TYPE":
		insertText(command_splitted[1], editor)
	case "SELECT":
		selectArea(command_splitted[1], editor)
	case "MOVE_CURSOR":
		moveCursor(command_splitted[1], editor)
	case "COPY":
		copy(editor)
	case "PASTE":
		paste(command_splitted, editor)
	case "EXIT":
		return true
	}
	return false
}

// main is the entry point of the program.
func main() {
	editor := Editor{"", "", 0, 0, 0, nil}
	var end bool = false
	reader := bufio.NewReader(os.Stdin)

	for {
		//Read input
		fmt.Print("Input: ")
		input, _ := reader.ReadString('\n')
		input = strings.Replace(input, "\r\n", "", -1)

		//Get all commands separated
		input_splitted := strings.Split(input, "\"")

		//Execute all operations
		for index, element := range input_splitted {
			if index%2 != 0 {
				end = executeOption(element, &editor)
			}

		}
		if end {
			break
		}

		//Print final editor's state
		fmt.Println("Output:", editor.text)
	}

	//Leave program
	fmt.Println("Leaving program...")

}
