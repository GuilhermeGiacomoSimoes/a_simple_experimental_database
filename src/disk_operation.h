#ifndef _DISK_OPERATION_H_ 
#define _DISK_OPERATION_H_ 

void disk_write(Page* page);
Page* disk_read(Page* page, int number_child);
void serialize_row(Row* source, void* destination);
void deserialize_row(void *source, Row* destination);

#endif
