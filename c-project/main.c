/**
 * @file main.c
 *
 * @brief Implementation of a command line based text editor
 *
 * @date 25/1/2024
 * @last_update 27/1/2024
 * @author Pol Guarch Bosom
 *
*/

// Include necessary standard C libraries
#include <stdio.h>
#include <string.h>

// Include the custom header file defining the Queue data structure and functions
#include "queue.h"

// Define constants for the length of specific operations
#define TYPE_LEN 4
#define PASTE_LEN 5

// Enumerated type representing clipboard operations
typedef enum {
    TYPE,
    SELECT,
    MOVE_CURSOR,
    COPY,
    PASTE,
    EXIT
} ClipboardOperation;

// Structure representing the Editor with its properties
typedef struct {
    char* text;           // Current text content in the editor
    char* selected;       // Selected text within the editor
    int start_selected;   // Starting index of the selected text
    int end_selected;     // Ending index of the selected text
    int cursor;           // Current cursor position in the editor
    char** clipboard;     // Array to store copied texts for pasting
    int n_clipboard;      // Number of texts in the clipboard
} Editor;

// Function prototypes
char*   readInput();
char**  split(char str[], char delim[], int* size);

void    insertText(Editor* editor, char* text);
void    deleteSelectedText(Editor* editor);

void    type(Editor* editor, char* operation);
void    selectArea(Editor* editor, char* operation);
void    moveCursor(Editor* editor, char* operation);
void    copy(Editor* editor);
void    paste(Editor* editor, char* operation);

ClipboardOperation  getOperation(char* opt);
int                 makeOperations(Queue* queue, Editor* editor);

void    freeEditor(Editor* editor);

/**
 * @brief Reads a line of input from the standard input.
 * 
 * @return line read from stin,
 */
char* readInput(){
    char temp;
    char * operation = NULL;
    int pos = 0;
    while(scanf("%c", &temp)){
        if(operation == NULL){
            operation = (char*)malloc(sizeof(char));
        }
        else{
            operation = (char*)realloc(operation, sizeof(char) * (pos+1));
        }
        if(temp == '\n'){
            operation[pos] = '\0';
            break;
        }
        operation[pos++] = temp;
    }
    return operation;
}

/**
 * @brief Splits a string into an array of substrings based on a delimiter.
 *
 * @param str The input string to be splitted.
 * @param delim The delimiter used for splitting the string.
 * @param size Pointer to an integer to store the number of substrings.
 * 
 * @return Array of substrings.
 */
char** split(char str[], char delim[], int* size){
    // Initialize the size if provided
    if(size != NULL) *size = 0;

    // Initialize the array with a single element
    char** list = (char**)malloc(sizeof(char *));
    int i = 0;

    if(list == NULL){
        return NULL;
    }

    // Tokenize the input string using the delimiter
    char* token = strtok(str,delim);
    
    if(token != NULL){
        do{
            // Allocate memory for each token
            list[i] = (char*)malloc(strlen(token)+1);

            if(!list[i]) return NULL;

            // Copy the token to the array
            strcpy(list[i++],token);

            // Update the size if provided
            if(size != NULL) (*size)++;

            // Get the next token
            token = strtok(NULL,delim);

            // Resize the array for additional tokens
            if(token != NULL){
                list = (char **)realloc(list, sizeof(char *) * (i + 1));
                if(list == NULL){
                    return NULL;
                }
            }
        }
        while(token != NULL);
    }
    return list;
}

/**
 * @brief Inserts text into the editor at the current cursor position.
 *
 * @param editor Pointer to the program editor.
 * @param text The text to be inserted.
 */
void insertText(Editor* editor, char* text){
    int new_size;
    char* aux_text;

    // Calculate the new size of the text after insertion and allocate memory for the new text
    new_size = strlen(editor->text) + strlen(text) + 1;
    aux_text = (char*)malloc(new_size);

    // Copy the text before the cursor from the original text
    strncpy(aux_text, editor->text, editor->cursor);

    // Insert the new text at the cursor position
    strcpy(aux_text + editor->cursor, text);

    // Copy the remaining text after the cursor from the original text
    strcpy(aux_text + editor->cursor + strlen(text), editor->text + editor->cursor);

    // Add null terminator to the end
    aux_text[new_size - 1] = '\0';

    // Update the editor's text and cursor
    free(editor->text);
    editor->text = aux_text;
    editor->cursor += strlen(text);

}

/**
 * @brief Deletes the currently selected text from the editor.
 * 
 * @param editor  Pointer to the program editor.
 */
void deleteSelectedText(Editor* editor){
    // Calculate the new size of the text after deletion
    int new_size = strlen(editor->text) - strlen(editor->selected) + 1;

    // Shift the remaining text to cover the deleted portion
    for(int j = editor->end_selected + 1; j < (int)strlen(editor->text); j++){
        (editor->text)[j - strlen(editor->selected)] = (editor->text)[j];
    }

    // Add null terminator to the end
    (editor->text)[new_size - 1]= '\0';

    // Resize the memory occupied by the editor's text
    editor->text = (char*)realloc(editor->text, new_size);

    // Reset selected information and update the cursor
    free(editor->selected);
    editor->selected = NULL;
    editor->cursor = editor->start_selected;
}

/**
 * @brief Handles the 'TYPE' operation by typing text into the editor.
 *
 * @param editor Pointer to the program editor.
 * @param operation The operation string containing the 'TYPE' operation and typped text.
 */
void type(Editor* editor, char* operation){
    // Extract the typed text from the operation string
    char* typped_text = (char*)malloc(strlen(operation) - (TYPE_LEN + 1) + 1);
    int i;
    for (i = TYPE_LEN + 1; i < (int)strlen(operation); i++){
        typped_text[i - (TYPE_LEN + 1)] = operation[i];
    }
    // Add null terminator to the end
    typped_text[i - (TYPE_LEN + 1)] = '\0';
    
    // Check if the editor's text is currently empty
    if (editor->text == NULL){
        // Set the text to the typed text and set the cursor position
        editor->text = strdup(typped_text);
        editor->cursor = i - (TYPE_LEN + 1);
    }
    else {
        // Check if there is a selected text, delete it, and insert the typed text
        if(editor->selected != NULL){
            deleteSelectedText(editor);
        }
        insertText(editor, typped_text);
    }
    free(typped_text);

}

/**
 * @brief Handles the 'SELECT' operation by selecting a range of text in the editor.
 * 
 * @param editor Pointer to the program editor.
 * @param operation The operation string containing the 'SELECT' operation and indices.
 */
void selectArea(Editor* editor, char* operation){
    // Split the operation string to extract indices
    char** command_splitted = split(operation, " ", NULL);
    int start_index = atoi(command_splitted[1]);
    int end_index = atoi(command_splitted[2]);

    // Free the memory occupied by the previous selected text and allocate for the new one
    if(editor->selected != NULL) free(editor->selected);
    editor->selected = (char*)malloc(end_index - start_index + 1 + 1);

    // Copy the selected text within the specified range
    int i;
    for(i = start_index; i<= end_index; i++){
        (editor->selected)[i - start_index] = (editor->text)[i];
    }
    // Add null terminator to the end
    (editor->selected)[i - start_index] = '\0';

    // Update cursor position and selected range information
    editor->cursor = end_index + 1;
    editor->start_selected = start_index;
    editor->end_selected = end_index;

    // Free the memory occupied by the split command
    free(command_splitted[0]);
    free(command_splitted[1]);
    free(command_splitted[2]);
    free(command_splitted);
}

/**
 * @brief Handles the 'MOVE_CURSOR' operation by moving the cursor in the editor.
 * 
 * @param editor Pointer to the program editor.
 * @param operation The operation string containing the 'MOVE_CURSOR' operation and offset.
 */
void moveCursor(Editor* editor, char* operation){
    // Split the operation string to extract the offset
    char** command_splitted = split(operation, " ", NULL);
    int offset = atoi(command_splitted[1]);

    // Update the cursor position by adding the offset and constrain the cursor to be within the valid range (0 to length of the text)
    editor->cursor += offset;
    editor->cursor = editor->cursor < 0? 0: editor->cursor;
    editor->cursor = editor->cursor > (int)strlen(editor->text)? (int)strlen(editor->text): editor->cursor;

    // Free the memory occupied by the selected text if present
    if(editor->selected != NULL){
        free(editor->selected);
        editor->selected = NULL;
    }

    // Free the memory occupied by the split command
    free(command_splitted[0]);
    free(command_splitted[1]);
    free(command_splitted);

}

/**
 * @brief Handles the 'COPY' operation by copying the selected text to the clipboard.
 *
 * @param editor Pointer to the program editor.
 */
void copy(Editor* editor){
    // Check if there is selected text in the editor
    if(editor->selected != NULL){
        // Check if the clipboard is empty, allocate memory if needed
        if(editor->n_clipboard == 0){
            editor->clipboard = (char**)malloc(sizeof(char*));
        }
        else{
            // Resize the clipboard to accommodate the new text
            editor->clipboard = (char**)realloc(editor->clipboard, sizeof(char*)* (editor->n_clipboard + 1));
        }
        // Copy the selected text to the clipboard and increment the clipboard count
        (editor->clipboard)[editor->n_clipboard] = strdup(editor->selected);
        (editor->n_clipboard)++;
    }
}

/**
 * @brief Handles the 'PASTE' operation by pasting text from the clipboard into the editor.
 * 
 * @param editor Pointer to the program editor.
 * @param operation The operation string containing the 'PASTE' operation and additional information.
 */
void paste(Editor* editor, char* operation){
    // Check if there is text in the clipboard
    if(editor->n_clipboard != 0){
        // Check if the operation indicates the text to paste
        if(strlen(operation) == PASTE_LEN){
            // Delete any selected text in the editor and insert the last entry from the clipboard
            if(editor->selected != NULL){
                deleteSelectedText(editor);
            }
            insertText(editor, (editor->clipboard)[editor->n_clipboard - 1]);
        }
        else{
            // Split the operation string to extract the steps back value
            char** command_splitted = split(operation, " ", NULL);
            int steps_back = atoi(command_splitted[1]);

            // Check if the steps back value is valid and within the clipboard range
            if(steps_back > 0 && steps_back <= editor->n_clipboard){
                // Delete any selected text in the editor and insert the specified entry from the clipboard
                if(editor->selected != NULL){
                    deleteSelectedText(editor);
                }
                insertText(editor, (editor->clipboard)[editor->n_clipboard - steps_back]);
            }

            // Free the memory occupied by the split command
            free(command_splitted[0]);
            free(command_splitted[1]);
            free(command_splitted);
        }
    }
}

/**
 * @brief Extracts the operation type from command and determines the ClipboardOperation type
 * 
 * @param opt Command read from queue
 * 
 * @return The ClipboardOperation enumeration type corresponding to the input.
 */
ClipboardOperation getOperation(char* opt) {
    int i = 0;
    char* option = NULL;

    // Extract the first word from the input string
    while(i < (int)strlen(opt) && opt[i] != ' '){
        if(option == NULL){
            option = (char*)malloc(1);
        }
        else{
            option = (char*)realloc(option, i+1);
        }
       
        option[i] = opt[i];
        i++;
    }

    // Allocate memory for the null terminator
    option = (char*)realloc(option, i+1);
    option[i] = '\0';

    // Compare the extracted option with known operation names
    if (strcmp(option, "TYPE") == 0) {
        free(option);
        return TYPE;
    }
    else if (strcmp(option, "SELECT") == 0) {
        free(option);
        return SELECT;
    }
    else if (strcmp(option, "MOVE_CURSOR") == 0) {
        free(option);
        return MOVE_CURSOR;
    }
    else if (strcmp(option, "COPY") == 0) {
        free(option);
        return COPY;
    }
    else if (strcmp(option, "PASTE") == 0) {
        free(option);
        return PASTE;
    }
    else{
        free(option);
        return EXIT;
    }
    
}

/**
 * @brief Executes the operations written in the input
 * 
 * @param queue Pointer to commands queue
 * @param editor Pointer to the program editor
 * 
 * @return 1 if exit operation is written, 0 otherwise.
 */
int makeOperations(Queue* queue, Editor* editor){
    char* operation = NULL;

    // Process commands from the queue until it is empty
    while(!QUEUE_isEmpty(*queue)){
        //Read command from queue
        operation = QUEUE_get(queue);

        // Perform operations based on the command type
        switch(getOperation(operation)){
            case TYPE:
                type(editor, operation);
                free(operation);
            break;
            case SELECT:
                selectArea(editor, operation);
                free(operation);
            break;
            case MOVE_CURSOR:
                moveCursor(editor, operation);
                free(operation);
            break;
            case COPY:
                copy(editor);
                free(operation);
            break;
            case PASTE:
                paste(editor, operation);
                free(operation);
            break; 
            case EXIT:
                free(operation);
                return 1;
        }
    }
    return 0;

}

/**
 * @brief Frees all memory used by editor
 * 
 * @param editor Pointer to the program editor
 */
void freeEditor(Editor* editor){
    if(editor->text != NULL) free(editor->text);
    if(editor->selected != NULL) free(editor->selected);
    for(int i = 0; i < editor->n_clipboard; i++){
        free((editor->clipboard)[i]);
    }
    if(editor->clipboard != NULL) free(editor->clipboard);
    

}

int main(){
    int end = 0;
    char* operation_list = NULL;
    char** operation_list_splitted = NULL;
    int size;

    Queue queue = QUEUE_init();
    Editor editor ={
        .text = NULL,
        .selected = NULL,
        .start_selected = 0,
        .end_selected = 0,
        .cursor = 0,
        .clipboard = NULL,
        .n_clipboard = 0
    };

    do{
        // Read input and split into commands
        printf("Input: ");
        operation_list = readInput();
        operation_list_splitted = split(operation_list, "\"", &size);

        for (int i = 0; i < size; i++){
            if(i%2 == 0) QUEUE_add(&queue, operation_list_splitted[i]);
            else free(operation_list_splitted[i]);
        }

        free(operation_list);
        free(operation_list_splitted);

        // Process operations from the queue
        end = makeOperations(&queue, &editor);

        // Print the final output
        printf("Output: %s\n", editor.text);
    }while(!end);

    // Print the final output
    printf("Leaving text editor...\n");

    // Free memory used by the editor and queue
    freeEditor(&editor);
    QUEUE_destroy(&queue);

    return 0;
}
