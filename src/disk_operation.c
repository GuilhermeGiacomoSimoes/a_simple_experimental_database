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
#define ID_OFFSET 0
#define USERNAME_OFFSET ID_OFFSET + ID_SIZE

#define LEAF_SIZE size_of_attribute(Page, leaf) 
#define ELEMS_SIZE size_of_attribute(Page, elems) 
#define ADDRESS_MEMMORY_SIZE size_of_attribute(Page, current_address_memory) 
#define LEAF_OFFSET 0 
#define ELEMS_OFFSET LEAF_OFFSET + LEAF_SIZE 
#define ADDRESS_MEMMORY_OFFSET ELEMS_OFFSET + ADDRESS_MEMMORY_SIZE 

void static serialize(Page* destination, Page_data* source) {
	if(source == NULL || destination == NULL) {
		exit(EXIT_FAILURE);
	}

	memset((void*)destination, 0, sizeof(destination));

	uint32_t n_rows = 0;

	for(uint32_t i = 0; i < MAX_ELEMENTS; i ++) {
		if(destination->info[i] != NULL) {
			n_rows+= 1;
		}
	}

	source->len = sizeof(uint32_t) + MAX_ELEMENTS + (3 + MAX_ELEMENTS) * sizeof(uint32_t) + n_rows * sizeof(Row);

	char* p = (char*)source;

	memcpy(p + sizeof(uint32_t), destination, 3 * sizeof(uint32_t));
	p += 4 * sizeof(uint32_t);

	mempy(p, &source->childs, MAX_ELEMENTS * sizeof(uint32_t));


	p += MAX_ELEMENTS * sizeof(uint32_t);


	for(uint32_t i = 0; i < MAX_ELEMENTS; i ++) {
		if(destination->info[i] == NULL) {
			*p++ = 0;
		}
		else {
			*p++ = 0xff;
			memcpy(p, destination->info[i], sizeof(Row));
			p += sizeof(Row);
		}
	}
}

void static deserialize(Page_data *source, Page* destination) {
	if(source == NULL || source == NULL) {
		exit(EXIT_FAILURE);
	}

	memset(destination, 0, sizeof(Page));
	memcpy(&destination->leaf, &source->p1, 3 * sizeof(uint32_t));
	memcpy(&destination->childs, &source->p2, MAX_ELEMENTS * sizeof(uint32_t));

	char *p = (char*) &source->p2;

	for(uint32_t i = 0; i < MAX_ELEMENTS; i ++) {
		if(*p++ == 0) {
			destination->info[i] = NULL;
		}
		else {
			destination->info[i] = (Row*) malloc(sizeof(Row));
			memcpy(destination->info[i], p, sizeof(Row));
			p += sizeof(Row);
		}
	}
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
	Page_data *serialized_page;
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
	x->current_address_memory = 0;
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

	Page* root;
	deserialize(serialized_root, &root);

	free(serialized_root);
	return root;
}

Page* read_a_child_page(Page* page, int number_child, int fd) {
	int memory_address_at_disk = page->childs[number_child];
	off_t offset = lseek(fd, memory_address_at_disk, SEEK_SET);
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
