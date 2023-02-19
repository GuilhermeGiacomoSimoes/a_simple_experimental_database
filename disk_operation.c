#include<stdio.h>
#include<fcntl.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include "structure.h"
#include "disk_operation.h"

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define NAME_FILE_DATABASE "database.db"

#define ID_SIZE size_of_attribute(Row, id)
#define DATA_SIZE size_of_attribute(Row, data)
#define ID_OFFSET 0
#define DATA_OFFSET ID_OFFSET + ID_SIZE

#define LEAF_SIZE size_of_attribute(Page, leaf) 
#define ELEMS_SIZE size_of_attribute(Page, elems) 
#define ADDRESS_MEMMORY_SIZE size_of_attribute(Page, current_address_memory) 
#define LEAF_OFFSET 0 
#define ELEMS_OFFSET LEAF_OFFSET + LEAF_SIZE 
#define ADDRESS_MEMMORY_OFFSET ELEMS_OFFSET + ADDRESS_MEMMORY_SIZE 

void static serialize(Row* source, void* destination) {
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + DATA_OFFSET, &(source->data), DATA_SIZE);
}

void static deserialize(void* destination, Row* source) {
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + DATA_OFFSET, &(source->data), DATA_SIZE);
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
		printf("Unable to open or create a file database\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(fd, page->current_address_memory, SEEK_SET);
	if(!page->current_address_memory) {
		offset = lseek(fd, 0, SEEK_END);
		page->current_address_memory = offset;
	}

	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE);
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	Page_data serialized_page;
	serialize(page, &serialized_page);
	ssize_t bytes_written = write(fd, &serialized_page, OFFSET_PAGE);

	if(bytes_written == -1) {
		printf("Error while try write file database\n");
		exit(EXIT_FAILURE);
	}
}

static Page* build_tree(int fd) {
	Page *x = malloc(sizeof(Page));
	x->leaf = 1;
	x->elems = 0;
	x->current_address_memory = 0;
	disk_write(x);
	return x;
}

Page* read_a_root_page(int fd) {
	off_t offset = lseek(fd, 0, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE); 
	}

	void *serialized_root = malloc(sizeof(Page_data));
	size_t bytes_read = read(fd, serialized_root, sizeof(Page_data));

	if(bytes_read == 0) {
		Page* root = build_tree(fd);
		return root; 
	}

	if(bytes_read == -1) {
		printf("Error while reading file database: \n");
		exit(EXIT_FAILURE);
	}

	Page root;
	deserialize(serialized_root, &root);

	free(serialized_root);
	return &root;
}

Page* read_a_child_page(Page* page, int number_child, int fd) {
	int memory_address_at_disk = page->childs[number_child];
	off_t offset = lseek(fd, memory_address_at_disk, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking\n");
	}

	void *serialized_child = malloc(sizeof(Page_data));
	size_t bytes_read = read(fd, serialized_child, sizeof(Page));

	if(bytes_read == -1) {
		printf("Error while reading file database\n");
		exit(EXIT_FAILURE);
	}

	Page child;
	deserialize(serialized_child, &child);

	free(serialized_child);
	return &child;
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
	else {
		return read_a_child_page(page, number_child, fd);
	}
}
