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
#define USERNAME_SIZE size_of_attribute(Row, username)
#define EMAIL_SIZE size_of_attribute(Row, email)
#define ID_OFFSET 0
#define USERNAME_OFFSET ID_OFFSET + ID_SIZE
#define EMAIL_OFFSET USERNAME_OFFSET + USERNAME_SIZE

#define LEAF_SIZE size_of_attribute(Page, leaf) 
#define ELEMS_SIZE size_of_attribute(Page, elems) 
#define ADDRESS_MEMMORY_SIZE size_of_attribute(Page, current_address_memmory) 
#define LEAF_OFFSET 0 
#define ELEMS_OFFSET LEAF_OFFSET + LEAF_SIZE 
#define ADDRESS_MEMMORY_OFFSET ELEMS_OFFSET + ADDRESS_MEMMORY_SIZE 

void static serialize_row(Row* source, void* destination) {
  	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  	memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  	memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void static deserialize_row(void* source, Row* destination) {
  	memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  	memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  	memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

ssize_t size(int *arr) {
	return sizeof(arr)/sizeof(int);
}

void static serialize(Page* source, void* destination) {
	memcpy(destination + LEAF_OFFSET, &(source->leaf), LEAF_SIZE);
	memcpy(destination + ELEMS_OFFSET, &(source->elems), ELEMS_SIZE);
	memcpy(destination + ADDRESS_MEMMORY_OFFSET, &(source->current_address_memmory), ADDRESS_MEMMORY_SIZE);

	off_t initial_serialize_info = ADDRESS_MEMMORY_OFFSET + ADDRESS_MEMMORY_SIZE;
	for(uint32_t index_info = 0; index_info < source->elems; index_info++) {
		void *row_serialize = malloc(sizeof(Row)); 
		serialize_row(&source->info[index_info], row_serialize);
		source->info[index_info] = row_serialize;

		memcpy(initial_serialize_info, source->info, sizeof(source->info));
		initial_serialize_info += sizeof(source->info);
	}
}

void static deserialize(void *source, Page* destination) {
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
	serialize(page, serialized_page);
	ssize_t bytes_written = write(fd, serialized_page, OFFSET_PAGE);

	if(bytes_written == -1) {
		printf("Error while try write file database\n");
		exit(EXIT_FAILURE);
	}
}

static Page* build_tree(int fd) {
	Page *x = malloc(sizeof(Page));
	x->leaf = 1;
	x->elems = 0;
	x->current_address_memmory = 0;
	disk_write(x);
	return x;
}

Page* read_a_root_page(int fd) {
	off_t offset = lseek(fd, 0, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking\n");
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	void *serialized_root = malloc(OFFSET_PAGE);
	size_t bytes_read = read(fd, serialized_root, OFFSET_PAGE);

	if(bytes_read == 0) {
		Page* root = build_tree(fd);
		return root; 
	}

	if(bytes_read == -1) {
		printf("Error while reading file database: \n");
		exit(EXIT_FAILURE);
	}

	Page* root = malloc(OFFSET_PAGE);
	deserialize(serialized_root, root);

	free(serialized_root);
	return root;
}

Page* read_a_child_page(Page* page, int number_child, int fd) {
	int memmory_address_at_disk = page->childs[number_child];
	off_t offset = lseek(fd, memmory_address_at_disk, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking\n");
	}

	const size_t OFFSET_PAGE = sizeof(Page); 
	void *serialized_child = malloc(OFFSET_PAGE);
	size_t bytes_read = read(fd, serialized_child, OFFSET_PAGE);

	if(bytes_read == -1) {
		printf("Error while reading file database\n");
		exit(EXIT_FAILURE);
	}

	Page* child = malloc(OFFSET_PAGE);
	deserialize(serialized_child, child);

	free(serialized_child);
	return child;
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

	return read_a_child_page(page, number_child, fd);
}
