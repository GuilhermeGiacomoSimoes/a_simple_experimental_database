#include<stdio.h>
#include<stdint.h>
#include "disk_operation.h"


void disk_write(Page* page) {
	if(page == NULL) {
		printf("Tried write a NULL page\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(
			NAME_FILE_DATABASE,
			O_RDWR | O_CREAT,
			S_IWUSR | S_IRUSR 
		);

	if(fd == -1) {
		printf("Unable to open of create a file database\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(fd, 0, SEEK_END);
	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	ssize_t bytes_written = write(fd, page, OFFSET_PAGE);

	if(bytes_written == -1) {
		printf("Error while try write file database%d\n", errno);
		exit(EXIT_FAILURE);
	}
}

Page* disk_read(Page* page, int number_child) {
	if(page == NULL) {
		printf("Tried read a NULL page\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(
			NAME_FILE_DATABASE,
			O_RDWR | O_CREAT,
			S_IWUSR | S_IRUSR 
		);

	if(fd == -1) {
		printf("Unable to open or create file\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(fd, page->childs[number_child], SEEK_SET);
	if(offset == -1) {
		printf("Error seeking: %d\n", errno);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	Page *child = malloc(OFFSET_PAGE);
	size_t bytes_read = read(fd, child, OFFSET_PAGE);

	if(bytes_read == -1) {
		printf("Error while reading file database: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	return child;
}
