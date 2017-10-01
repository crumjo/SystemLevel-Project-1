//
//  file_utils.c
//  Cipher
//
//  Created by Joshua Crum on 9/25/17.
//  Copyright © 2017 Joshua Crum. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "file_utils.h"

int fileEncrypt( char* key, char* filename ) {
    if ( access( filename, F_OK) == -1) {
        fprintf(stderr, "The file '%s' cannot be found or does not exist.\n\n", filename);
        exit(0);
    }
    
    char* tempKey = key;
    long len = strlen(key);
    char* buffer = malloc(len * sizeof(char));
    for (int i = 0; i < len; i++) {
        
    }
    
    return 0;
}


int fileDecrypt( char* key, char* filename ) {
    
    return 0;
}


char lowerAsciiFind(char* key, char find) {
    char temp;
    long len = strlen(key) + 65;
    int j = 0;
    
    for (int i = 65; i < len; i++) {
        
    }
    
    return temp;
}
//
//
//char upperAsciiFind(char* key, char find) {
// 
//    return 'a';
//}


