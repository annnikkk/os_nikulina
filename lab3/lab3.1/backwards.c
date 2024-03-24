#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>


void CreatingDir(char* dir_name){
    if(mkdir(dir_name, 0777) == 0){
        printf("dir created successfully\n");
    } else {
        printf("error in creating dir");
    }
}

char* Reverse(const char* dir_name, int dir_len){
    char* new_name = (char*) malloc(sizeof(char) * dir_len + 1);
    int index = 0;
    for(int i = dir_len - 1; i >= 0; i--){
        new_name[index] = dir_name[i];
        index++;
    }
    new_name[index] = '\0';
    return new_name;
}

void RewritingFile(FILE* from_file, FILE* to_file){
    fseek(from_file, 0, SEEK_END);
    long file_size = ftell(from_file);
    fseek(from_file, 0, SEEK_SET);
    char* buffer = (char*) malloc(sizeof(char) * file_size);
    size_t bytes_read = fread(buffer, 1, file_size, from_file);
    size_t bytes_written = 0;
    for(int i = file_size - 1; i >= 0; i--){
        bytes_written += fwrite(&buffer[i], 1, 1, to_file);
    }
    if(bytes_written != bytes_read){
        printf("error in rewriting file\n");
    }
    free(buffer);
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("wrong arguments\n");
        return 1;
    }
    char* dir_path = (char*) malloc(sizeof(char) * 1024);
    snprintf(dir_path, 1024, "%s/%s", argv[1], argv[2]);
    DIR* dir = opendir(dir_path);
    if(dir == NULL){
        printf("error in opening dir\n");
        return 1;
    }
    
    char* dir_name = argv[2];
    int dir_len = strlen(dir_name);
    
    dir_name = Reverse(dir_name, dir_len);
    CreatingDir(dir_name);
    
    struct dirent* cur_file;
    while((cur_file = readdir(dir)) != NULL){
        if(cur_file->d_type == DT_REG){
            char* file_name = Reverse(cur_file->d_name, strlen(cur_file->d_name));
            char* file_path = (char*) malloc(sizeof(char) * 1024);
            snprintf(file_path, 1024, "%s/%s/%s", argv[1], argv[2], cur_file->d_name);
            char* new_file_path = (char*) malloc(sizeof(char) * 1024);
            snprintf(new_file_path, 1024, "%s/%s/%s", argv[1], dir_name, file_name);
            FILE* original_file = fopen(file_path, "rb");
            FILE* new_file = fopen(new_file_path, "wb");
            if(original_file == NULL || new_file == NULL){
                printf("error in opening files\n");
                return 1;
            }
            RewritingFile(original_file, new_file);
            fclose(original_file);
            fclose(new_file);
            free(file_path);
            free(new_file_path);
        }
    }
    free(dir_path);
    closedir(dir);
    return 0;
}