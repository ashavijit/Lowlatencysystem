/*                  Memory Mapping

it provides a way to map a files's content directly into main memory.
this can improve the efficiency of reading and writing data,
especially when dealing with large datasets, as it reduces the need for explicit I/O operations
Memory-mapped files can be advantageous in low-latency systems where quick data access is critical.

(c) Avijit Sen
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MY_FILE "socket.c"
#define FILE_SIZE 1024

int main(){
    int fp;
    char *mapped_data;

    fp = open(MY_FILE,O_RDWR | O_CREAT | O_TRUNC , (mode_t)0600);
    if(fp == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    // expand it
    if(lseek(fp,FILE_SIZE-1,SEEK_SET) == -1){
        perror("lseek");
        close(fp);
        exit(EXIT_FAILURE);
    }
    if(write(fp,"",1) == -1){
        perror("write");
        close(fp);
        exit(EXIT_FAILURE);
    }

    mapped_data = mmap(0,FILE_SIZE,PROT_READ | PROT_WRITE ,MAP_SHARED,fp,0);
    if(mapped_data == MAP_FAILED){
        perror("mmap");
        close(fp);
        exit(EXIT_FAILURE);
    }

    strcpy(mapped_data, "// this is a comment done by memory mapping!");
    if(msync(mapped_data,FILE_SIZE,MS_SYNC) == -1){
        perror("msync");
    }
    if (munmap(mapped_data, FILE_SIZE) == -1)
    {
        perror("munmap");
    }
    close(fp);

    // Reopen the file for reading
    fp = open(MY_FILE, O_RDONLY);
    if (fp == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    mapped_data = mmap(0, FILE_SIZE, PROT_READ, MAP_SHARED, fp, 0);
    if (mapped_data == MAP_FAILED)
    {
        perror("mmap");
        close(fp);
        exit(EXIT_FAILURE);
    }

    printf("Data from memory-mapped file: %s\n", mapped_data);

    if (munmap(mapped_data, FILE_SIZE) == -1)
    {
        perror("munmap");
    }
    close(fp);

    return 0;
}