#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 1024

struct info{
    size_t size; //то, сколькими байтами в данный момент распоряжается структура
    struct info* next;
    char is_available;
};

void* heap_start;

void prepare(){
    void* heap = mmap(NULL, MAX_HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(heap == MAP_FAILED){
        perror("error in mmap");
        exit(1);
    }
    heap_start = heap;
    ((struct info*) heap_start)->size = MAX_HEAP_SIZE - sizeof(struct info);
    ((struct info*) heap_start)->is_available = 1;
    ((struct info*) heap_start)->next = NULL;
}

void* my_malloc(size_t size){
    if(heap_start == NULL) prepare();
    void* memory = NULL;
    
    struct info* cur_info = (struct info*) heap_start;
    
    while(cur_info != NULL){
        printf("wanted size: %lu, is available = %d, size = %lu\n", size, cur_info->is_available, cur_info->size);
        if(cur_info->is_available && cur_info->size >= size){
            if(size + sizeof(struct info) >= cur_info->size){
                cur_info->size = size;
                cur_info->is_available = 1;
                break;
            }
            struct info* next = (struct info*) ((char*) cur_info + sizeof(struct info) + size);
            if((char*) next >= (char*) heap_start + MAX_HEAP_SIZE) cur_info->next = NULL;
            else {
                next->size = cur_info->size - sizeof(struct info) - size;
                next->is_available = 1;
                
                if(cur_info->next != NULL) next->next = cur_info->next;
                cur_info->next = next;
            }
            
            cur_info->size = size;
            cur_info->is_available = 0;
            
            memory = (void*) (cur_info + 1);
            break;
        }
        cur_info = cur_info->next;
    }
    
    if(memory == NULL){
        fprintf(stderr, "malloc: cant find a memory\n");
        return NULL;
    }
    
    printf("give %lu of memory\n", size);
    return memory;
}

void my_free(void* memory){
    if(memory == NULL){
        return;
    }
    struct info* to_free = (struct info*) memory - 1;
    if(to_free->is_available == 1){
        fprintf(stderr, "free: its a freed memory\n");
        return;
    }
    
    to_free->is_available = 1;
    if(to_free->next != NULL && to_free->next->is_available){
        to_free->size += to_free->next->size + sizeof(struct info);
        to_free->next = to_free->next->next;
    }
}

int main(){
    char* arr1 = my_malloc(100);
    if(arr1 == NULL) return 1;
    char* arr = my_malloc(400);
    if(arr == NULL) return 2;
    my_free(arr1);
    char* arr2 = my_malloc(50);
    if(arr2 == NULL) return 3;
    char* arr3 = my_malloc(50);
    if(arr3 == NULL) return 4;
    my_free(arr);
    my_free(arr2);
    my_free(arr3);
    return 0;
}