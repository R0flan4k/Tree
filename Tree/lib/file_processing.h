#ifndef FILE_PROCESSING_H
    #define FILE_PROCESSING_H

    #include <stdio.h>

    // long get_file_size(FILE * fp);
    FILE * file_open(const char * file_name, const char * mode);
    char * read_file(char * buffer, const size_t buffer_size, FILE * fp);
    size_t write_file(char * buffer, const size_t buffer_size, FILE * fp);

#endif // FILE_PROCESSING_H
