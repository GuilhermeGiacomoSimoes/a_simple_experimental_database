#include<stdio.h>
#include<stdint.h>

#define NAME_FILE_DATABASE "database.db";

void disk_write() {

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
