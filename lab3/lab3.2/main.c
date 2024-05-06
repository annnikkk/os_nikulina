#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


int CreateDir(const char* dir_name){
    if(mkdir(dir_name, 0777) != 0){
        perror("error in creating dir\n");
        return 1;
    }
    return 0;
}

void ListDir(const char* dir_name){
    DIR* dir = opendir(dir_name);
    if(dir == NULL){
        perror("error in opening dir\n");
        return;
    }
    struct dirent* cur_file;
    while((cur_file = readdir(dir)) != NULL){
        printf("%s\n", cur_file->d_name);
    }
    closedir(dir);
}

void DeleteDir(const char* dir_name){
    if(rmdir(dir_name) != 0){
        perror("error in deleting dir\n");
        return;
    }
}

int CreateFile(const char* file_name){
    FILE* file = fopen(file_name, "w+");
    if(file == NULL){
        perror("error in creating file\n");
        return 1;
    }
    fprintf(file, "This is content of file\n");
    if(fclose(file) != 0){
        perror("error in closing file");
        return 1;
    }
    return 0;
}

void PrintFile(const char* file_name){
    FILE* file = fopen(file_name, "rb");
    if(file == NULL){
        perror("error in opening file");
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*) malloc(sizeof(char) * file_size);
    if(buffer == NULL){
        perror("error in malloc");
        return;
    }
    size_t bytes_read = fread(buffer, 1, file_size, file);
    if(bytes_read != file_size){
        perror("error in reading file\n");
        free(buffer);
        if(fclose(file) != 0){
            perror("error in closing file");
        }
        return;
    }
    for(int i = 0; i < file_size; i++){
        printf("%c", buffer[i]);
    }
    free(buffer);
    if(fclose(file) != 0){
        perror("error in closing file");
        return;
    }
}

void DeleteFile(const char* file_name){
    if(remove(file_name) != 0){
        perror("error in deleting file\n");
        return;
    }
}

int CreateSymlink(char* file_name, char* link_name){
    if(symlink(file_name, link_name) != 0){
        perror("error in creating symlink\n");
        return 1;
    }
    return 0;
}

void PrintSymlink(const char* link_name){
    char* buffer = (char*) malloc(sizeof(char) * 1024);
    if(buffer == NULL){
        perror("error in malloc");
        return;
    }
    size_t bytes_read = readlink(link_name, buffer, sizeof(buffer));
    if(bytes_read == -1){
        perror("error in reading symlink\n");
        return;
    }
    buffer[bytes_read] = '\0';
    printf("%s\n", buffer);
    free(buffer);
}

void PrintFileFromSymlink(const char* link_name){
    char* buffer = (char*) malloc(sizeof(char) * 1024);
    if(buffer == NULL){
        perror("error in malloc");
        return;
    }
    size_t bytes_read = readlink(link_name, buffer, sizeof(buffer));
    if(bytes_read == -1){
        perror("error in reading symlink\n");
        return;
    }
    buffer[bytes_read] = '\0';
    PrintFile(buffer);
    free(buffer);
}

void DeleteSymlink(const char* link_name){
    if(unlink(link_name) == -1){
        perror("error in deleting symlink\n");
        return;
    }
}

int CreateHardlink(const char* file_name, const char* link_name){
    if(link(file_name, link_name) == -1){
        perror("error in creating hardlink\n");
        return 1;
    }
    return 0;
}

void DeleteHardlink(char* link_name){
    if(unlink(link_name) == -1){
        perror("error in deleting hardlink\n");
        return;
    }
}

void PrintInfo(char* file_name){
    printf("I'm in PrintInfo\n");
    struct stat file_stat;
    if(stat(file_name, &file_stat) != 0){
        perror("error in getting info");
        return;
    }
    char* mode = (char*) malloc(sizeof(char) * 11);
    if(mode == NULL){
        perror("error in malloc");
        return;
    }
    for(int i = 0; i < 9; i++){
        mode[i] = '-';
    }
    mode[10] = '\0';
    
    if(S_ISDIR(file_stat.st_mode)) mode[0] = 'd';
    else if(S_ISLNK(file_stat.st_mode)) mode[0] = 'l';
    
    if((file_stat.st_mode & S_IRUSR) != 0) mode[1] = 'r';
    if((file_stat.st_mode & S_IWUSR) != 0) mode[2] = 'w';
    if((file_stat.st_mode & S_IXUSR) != 0) mode[3] = 'x';
    if((file_stat.st_mode & S_IRGRP) != 0) mode[4] = 'r';
    if((file_stat.st_mode & S_IWGRP) != 0) mode[5] = 'w';
    if((file_stat.st_mode & S_IXGRP) != 0) mode[6] = 'x';
    if((file_stat.st_mode & S_IROTH) != 0) mode[7] = 'r';
    if((file_stat.st_mode & S_IWOTH) != 0) mode[8] = 'w';
    if((file_stat.st_mode & S_IXOTH) != 0) mode[9] = 'x';
    
    for(int i = 0; i < 9; i++){
        printf("%c", mode[i]);
    }
    printf("\n");
    free(mode);
}

void ChangeMode(char* file_name, char* new_mode){
    mode_t mode = 0;
    for(int i = 0; i < strlen(new_mode); i++){
        if(new_mode[i] == 'r' && i == 1) mode |= S_IRUSR;
        else if(new_mode[i] == 'r' && i == 4) mode |= S_IRGRP;
        else if(new_mode[i] == 'r' && i == 7) mode |= S_IROTH;
        
        else if(new_mode[i] == 'w' && i == 2) mode |= S_IWUSR;
        else if(new_mode[i] == 'w' && i == 5) mode |= S_IWGRP;
        else if(new_mode[i] == 'w' && i == 8) mode |= S_IWOTH;
        
        else if(new_mode[i] == 'x' && i == 3) mode |= S_IXUSR;
        else if(new_mode[i] == 'x' && i == 6) mode |= S_IXGRP;
        else if(new_mode[i] == 'x' && i == 9) mode |= S_IXOTH;
        
        else if(new_mode[i] == '-') continue;
        else {
            perror("incorrect mode symbol");
        }
    }
    if(chmod(file_name, mode) == -1){
        perror("error in chanding mode");
        return;
    }
}

int main(int argc, char* argv[]){
    if(argc < 4){
        printf("wrong arguments\n use: <dir_name> <file_name> <symlink_name> <hardlink_name>");
        return 1;
    }
    
    char* dir_name = argv[1];
    if(CreateDir(dir_name) == 1) return 1;
    ListDir(dir_name);
    DeleteDir(dir_name);
    
    char* file_name = argv[2];
    if(CreateFile(file_name) == 1) return 1;
    PrintInfo(file_name);
    char* new_mode = "-rwxrwxrwx";
    ChangeMode(file_name, new_mode);
    
    PrintInfo(file_name);
    PrintFile(file_name);
    
    char* symlink_name = argv[3];
    if(CreateSymlink(file_name, symlink_name) == 1) return 1;
    PrintSymlink(symlink_name);
    DeleteSymlink(symlink_name);
    
    char* hardlink_name = argv[4];
    if(CreateHardlink(file_name, hardlink_name) == 1) return 1;
    DeleteHardlink(hardlink_name);
    
    DeleteFile(file_name);
    
    return 0;
}