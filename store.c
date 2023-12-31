#include "common.h"
#include "store.h"
#include "disk_operation.h"

result insert(row* data)
{
	disk_write(data);
}


result search(uint32_t we) 
{
	result r;
	r.code = 1;
	r.description = "sucesso meu nobre";
	return r;
}

