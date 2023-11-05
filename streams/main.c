#include <stdio.h>

int main() {
    FILE *file;
    char buffer[100];

    // Open a file for reading
    file = fopen("example.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and print lines from the file
    while (my_fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Close the file
    fclose(file);

    return 0;
}


char *my_fgets(char *buffer, int size, FILE *stream) {
    int ch;
    int i = 0;

    // Ensure buffer is not NULL and size is valid
    if (buffer == NULL || size <= 0) {
        return NULL;
    }

    // Read characters from the stream until size - 1 characters are read,
    // or until a newline character is encountered, or until EOF is reached.
    while (i < size - 1) {
        ch = getc(stream);
        
        if (ch == EOF) {
            // End of file reached or an error occurred
            if (i == 0) {
                // No characters were read, return NULL to indicate failure
                return NULL;
            } else {
                // Return the buffer with the read characters
                buffer[i] = '\0';
                return buffer;
            }
        } else if (ch == '\n') {
            // Newline encountered, include it in the buffer
            buffer[i++] = (char)ch;
            break;
        } else {
            // Regular character, add it to the buffer
            buffer[i++] = (char)ch;
        }
    }

    // Null-terminate the buffer and return it
    buffer[i] = '\0';
    return buffer;
}