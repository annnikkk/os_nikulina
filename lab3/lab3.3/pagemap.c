#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PAGE_SIZE 4096
#define PFN_MASK (((uint64_t)1 << 55) - 1)
#define PAGE_PRESENT ((uint64_t)1 << 63)


int extract_number(const char* line, uint64_t* value, char** end_ptr){
    errno = 0;
    
    *value = strtoull(line, end_ptr, 16);
    if(errno != 0){
        perror("error in strtoull");
        return 1;
    }
    if(*end_ptr == line){
        fprintf(stderr, "strtoull: no digits were found\n");
        return 1;
    }
    
    return 0;
}

int parse_maps_entry(const char* maps_entry, uint64_t* start_addr, uint64_t* end_addr){
    int ret = 0;
    char* end_ptr = NULL;
    
    ret = extract_number(maps_entry, start_addr, &end_ptr);
    if(ret == 1){
        return 1;
    }
    ret = extract_number(end_ptr + 1, end_addr, &end_ptr);
    
    return ret;
}

int print_pagemap_entries(int pagemap_fd, uint64_t start_addr, uint64_t end_addr){
    for(uint64_t addr = start_addr; addr < end_addr; addr += PAGE_SIZE){
        uint64_t page_num = addr / PAGE_SIZE;
        uint64_t pagemap_entry;
        ssize_t ret;
        
        ret = pread(pagemap_fd, &pagemap_entry, sizeof(pagemap_entry), page_num * sizeof(pagemap_entry));
        if(ret == 1){
            perror("error in pread");
            return 1;
        }
        
        printf("0x%lx -> 0x%lx\n", addr, pagemap_entry & PFN_MASK);
    }
    return 0;
}

int print_pagemap(){
    int ret = 0;
    int pagemap_fd = 0;
    char* pagemap_file = "/proc/self/pagemap";
    char* maps_file = "/proc/self/maps";
    FILE* maps_fp = NULL;
    
    maps_fp = fopen(maps_file, "r");
    if(maps_fp == NULL){
        perror("error in fopen");
        return EXIT_FAILURE;
    }
    
    pagemap_fd = open(pagemap_file, O_RDONLY);
    if(pagemap_fd == 1){
        perror("error in open");
        return EXIT_FAILURE;
    }
    
    while(true){
        uint64_t start_addr;
        uint64_t end_addr;
        char maps_entry[BUFFER_SIZE] = {0};
        char* ptr_ret = NULL;
        
        ptr_ret = fgets(maps_entry, BUFFER_SIZE, maps_fp);
        if(ptr_ret == NULL){
            ret = ferror(maps_fp);
            if(ret != 0){
                perror("error in fscanf");
                ret = 1;
            }
            
            break;
        }
        
        ret = parse_maps_entry(maps_entry, &start_addr, &end_addr);
        if(ret == 1){
            break;
        }
        
        ret = print_pagemap_entries(pagemap_fd, start_addr, end_addr);
        if(ret == 1){
            break;
        }
    }
    
    fclose(maps_fp);
    close(pagemap_fd);
    
    return ret;
}


int main(){
    int ret = print_pagemap();
    
    if(ret == 1){
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}