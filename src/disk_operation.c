#include<stdio.h>
#include<stdint.h>
#include "structure.h"

void serialize_row(Row* source, void* destination) {
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
	memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void *source, Row* destination) {
	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

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

	off_t offset = lseek(fd, page->current_address_memmory, SEEK_SET);
	if(!page->current_address_memmory) {
		offset = lseek(fd, 0, SEEK_END);
		page->current_address_memmory = offset;
	}

	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	void *serialized_page = malloc(OFFSET_PAGE);
	serialize_row(page, serialized_page);
	ssize_t bytes_written = write(fd, serialized_row, OFFSET_PAGE);

	if(bytes_written == -1) {
		printf("Error while try write file database%d\n", errno);
		exit(EXIT_FAILURE);
	}
}

Page* disk_read(Page* page, int number_child) {
	int fd = open(
			NAME_FILE_DATABASE,
			O_RDWR | O_CREAT,
			S_IWUSR | S_IRUSR 
		);

	if(fd == -1) {
		printf("Unable to open or create file\n");
		exit(EXIT_FAILURE);
	}

	if(page == NULL) {
		return read_a_root_page(fd);
	}

	return read_a_child_page(Page* page, number_child, fd);
}

Page* read_a_root_page(int fd) {
	off_t offset = lseek(fd, 0, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking: %d\n", errno);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	void *serialized_root = malloc(OFFSET_PAGE);
	size_t bytes_read = read(fd, root, OFFSET_PAGE);

	if(bytes_read == -1) {
		printf("Error while reading file database: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	Page* root = malloc(OFFSET_PAGE);
	deserialize_row(serialized_root, root);

	free(serialized_root);
	return root;
}

Page* read_a_child_page(Page* page, int number_child, int fd) {
	int memmory_address_at_disk = page->childs[number_child];
	off_t offset = lseek(fd, memmory_address_at_disk, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking: %d\n", errno);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	void *serialized_child = malloc(OFFSET_PAGE);
	size_t bytes_read = read(fd, serialized_child, OFFSET_PAGE);

	if(bytes_read == -1) {
		printf("Error while reading file database: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	Page* child = malloc(OFFSET_PAGE);
	deserialize_row(serialized_child, child);

	free(serialized_child);
	return child;
}
