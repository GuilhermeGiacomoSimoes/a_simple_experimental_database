#ifndef _DISK_OPERATION_H_ 
#define _DISK_OPERATION_H_ 

#include "store.h"

void disk_write(const row_t* data);
page_t disk_read();

#endif
