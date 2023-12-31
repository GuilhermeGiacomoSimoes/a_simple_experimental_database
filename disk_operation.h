#ifndef _DISK_OPERATION_H_ 
#define _DISK_OPERATION_H_ 

#include "store.h"

void disk_write(row* data);
page disk_read();

#endif
