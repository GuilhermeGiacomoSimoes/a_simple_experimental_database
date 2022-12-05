#ifndef _DISK_OPERATION_H_ 
#define _DISK_OPERATION_H_ 

void disk_write(Page* page);
Page* disk_read(Page* page, int number_child);

#endif
