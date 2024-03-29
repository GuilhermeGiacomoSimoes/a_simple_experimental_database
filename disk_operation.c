#include<stdio.h>
#include<fcntl.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include "store.h"
#include "disk_operation.h"

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define NAME_FILE_DATABASE "database.db"

#define ID_SIZE size_of_attribute(row_t, id)
#define DATA_SIZE size_of_attribute(row_t, data)
#define ID_OFFSET 0
#define DATA_OFFSET ID_OFFSET + ID_SIZE

void static serialize(const row_t* source, char* destination) 
{
	memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination + DATA_OFFSET, &(source->data), DATA_SIZE);
}

void static deserialize(row_t* source, void* destination) 
{
	memcpy(&(source->id), destination + ID_OFFSET, ID_SIZE);
	memcpy(&(source->data), destination + DATA_OFFSET, DATA_SIZE);
}

void disk_write(const row_t* data)
{
	int fd = open(
			NAME_FILE_DATABASE,
			O_RDWR | O_CREAT,
			S_IWUSR | S_IRUSR 
		);

	if(fd == -1) {
		printf("Unable to open or create a file database\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(fd, 0, SEEK_END);
	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE);
	}

	char *serialized_data = malloc(sizeof(*data));
	serialize(data, serialized_data);
	ssize_t bytes_written = write(fd, serialized_data, ID_SIZE + DATA_SIZE);

	free(serialized_data);

	if(bytes_written == -1) {
		printf("Error while try write file database\n");
		exit(EXIT_FAILURE);
	}
}

page_t disk_read() 
{
	int fd = open(
			NAME_FILE_DATABASE,
			O_RDWR | O_CREAT,
			S_IWUSR | S_IRUSR 
		);

	if(fd == -1) {
		printf("Unable to open or create file\n");
		exit(EXIT_FAILURE);
	}

	off_t offset = lseek(fd, 0, SEEK_SET);
	if(offset == -1) {
		printf("Error seeking\n");
		exit(EXIT_FAILURE); 
	}

	void *serialized_page = malloc(sizeof(page_t));
	size_t bytes_read = read(fd, serialized_page, sizeof(page_t));
	
	page_t pp;
	for(uint16_t idx = 0 ; idx < MAX_ELEMENTS; idx ++) {
		row_t* r = (row_t*) malloc(sizeof(row_t));
		deserialize(serialized_page, r);
		pp.info[idx] = r;
	}

	return pp;
}
