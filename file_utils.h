//
//  file_utils.h
//  Cipher
//
//  Created by Joshua Crum on 9/25/17.
//  Copyright © 2017 Joshua Crum. All rights reserved.
//

#ifndef file_utils_h
#define file_utils_h

int read_file( char* filename, char **buffer );
int write_file( char* filename, char *buffer, int size );
int write_file_unsafe( char* filename, char *buffer, int size );
int fileEncrypt( char* buffer, char* key, int size );
int fileDecrypt( char* buffer, char* key, int size );

#endif /* file_utils_h */
