#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "file_utils.h"



/*****************************************************************
 Read the contents of the file into the buffer and calculate the
 size of the file.
 
 @param filename the name of the file to be read.
 @param buffer the pointer to a pointer to read the file into.
 @return int the size of the file.
 *****************************************************************/
int read_file( char* filename, char **buffer )
{
    
    /** The number of elements in the file. */
    int size = 0;
    
    if (access(filename, F_OK) == -1) {
        fprintf(stderr, "\nThe file '%s' cannot be found "
                "or does not exist.\n\n", filename);
        return -1;
        
    } else {
        
        /** Calculate file size, provided by Professor Woodring */
        struct stat st;
        stat(filename, &st);
        size = st.st_size;
        
        **buffer = (char)malloc(size * sizeof(char));
        if ( buffer == NULL ) {
            fprintf(stderr, "\nMemory allocation failed.\n");
            return -1;
        }
        
        /** Open the file in read mode. */
        FILE *in_file = fopen( filename, "r" );
        if (in_file == NULL) {
            fprintf(stderr, "File open failed.");
            fclose(in_file);
            return -1;
        }
        
        fread(*buffer, sizeof(char), size, in_file);
        fclose(in_file);
    }
    
    return size;
}


/*****************************************************************
 Write the contents of the buffer into a new or existing file.
 
 @param filename the name of the output file to write to.
 @param buffer the char array from which to write.
 @param size the number of elements to write to the output file.
 @return int 0 if the code executed correctly, 1 otherwise.
 *****************************************************************/
int write_file( char* filename, char *buffer, int size )
{
    
    if ( access( filename, F_OK) != -1 ) {
        
        /** Capture user input for choice to overwrite a file. */
        char x;
        printf("'%s' already exists, would you like to replace it?"
               " (y or n)\n", filename);
        scanf("%c", &x);
        
        while ( x != 'n' && x != 'N' && x != 'y' && x != 'Y') {
            printf("Invalid input, enter y or n.\n");
            scanf(" %c", &x);
        }
        
        if (x == 'n' || x == 'N') {
            printf("\nNo changes to '%s' were made."
                   "\n\n", filename);
            return -1;
            
        } else if (x == 'y' || x == 'Y') {
            printf("'%s' has been overwritten.\n", filename);
        }
        
    }
    
    /** Create an output file in write mode. */
    FILE *out_file = fopen(filename, "w");
    if (out_file == NULL) {
        fprintf(stderr, "File open failed.");
        return -1;
    }
    
    buffer[size] = '\0';
    fwrite(buffer, sizeof(char), size, out_file);
    fclose(out_file);
    
    return 0;
}


/*****************************************************************
 Write the contents of the buffer into a new or existing file
 without checking for overwrites.
 
 @param filename the name of the output file to write to.
 @param buffer the char array from which to write.
 @param size the number of elements to write to the output file.
 @return int 0 if the code executed correctly, 1 otherwise.
 *****************************************************************/
int write_file_unsafe( char* filename, char *buffer, int size )
{
    /** Create an output file in write mode. */
    FILE *out_file = fopen(filename, "w");
    if (out_file == NULL) {
        fprintf(stderr, "File open failed.");
        return -1;
    }
    
    buffer[size] = '\0';
    fwrite(buffer, sizeof(char), size, out_file);
    fclose(out_file);
    
    return 0;
}



/*****************************************************************
 Encrypt a file given a key.
 
 @param buffer the contents of a file stored in a pointer.
 @param key the cipher key to use for encryption.
 @param size the length of the buffer.
 @return int 0 if the funcion properly executed.
 *****************************************************************/
int fileEncrypt( char* buffer, char* key, int size )
{
    size_t key_len = strlen(key) - 1;
    char* position = (char*)malloc(size * sizeof(char));
    
    for (int i = 0; i < size; i++) {
        if ((buffer[i] >= 65 && buffer[i] <= 90) || (buffer[i] >= 97 && buffer[i] <= 122)) {
        
            int buff_pos = buffer[i];
        
            if (islower(buffer[i])) {
                if ((buff_pos - 97) <= key_len) {
                    int x = buff_pos - 97;
                    buffer[i] = tolower(key[x]);
                    
                    sprintf(position + strlen(position), "%d ", i);
                }
            
            } else if (isupper(buffer[i])) {
                if ((buff_pos - 65) <= key_len) {
                    int y = buff_pos - 65;
                    buffer[i] = toupper(key[y]);
                    
                    sprintf(position + strlen(position), "%d ", i);
                }
            }
        }
    }
    
    write_file_unsafe("position.txt", position, strlen(position));
    
    free(position);
    return 0;
}


/*****************************************************************
 Decrypts a file given a key.
 
 @param buffer the contents of a file stored in a pointer.
 @param key the cipher key to use for decryption.
 @param size the length of the buffer.
 @return int 0 if the funcion properly executed.
 *****************************************************************/
int fileDecrypt( char* buffer, char* key, int size )
{
    size_t key_len = strlen(key);
    
    char* position = (char*)malloc(sizeof(char));
    int pos = read_file("position.txt", &position);
    int* int_position = (int*)malloc(pos * sizeof(int));
    
    /** Help from stack overflow reading file into int array. */
    FILE *pos_file = fopen("position.txt", "r");
    int array[size];
    int i = 0, retval;
    while(i < size && (retval = fscanf(pos_file, "%d", &array[i++])) == 1) ;
    
    if(retval == EOF) {
        if(ferror(pos_file)) {
            fprintf(stderr, "Read error in stream.");
            clearerr(pos_file);
        }
    }
    
    int increment = 0;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < key_len; j++) {
            
            int inc = array[increment];
            
            if (i == inc) {
                if (islower(buffer[i])) {
                    if (tolower(buffer[i]) == tolower(key[j])) {
                        int x = buffer[i] - 97;
                        int y = key[j] - x + j;
                    
                        buffer[i] = tolower(y);
                    
                        j = key_len + 1;
                        increment++;
                    }
                
                } else if (isupper(buffer[i])) {
                    if (toupper(buffer[i]) == toupper(key[j])) {
                        int x = buffer[i] - 65;
                        int y = key[j] - x + j;
                    
                        buffer[i] = toupper(y);
                    
                        j = key_len + 1;
                        increment++;
                    }
                }
            }
        }
    }
   
    free(position);
    free(int_position);
    return 0;
}
