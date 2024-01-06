#include "common.h"
#include "store.h"
#include "disk_operation.h"

result_t insert(row_t* data)
{
	disk_write(data);
}


result_t search(uint32_t we) 
{
	result_t r;
	r.code = 1;
	r.description = "sucesso meu nobre";
	return r;
}

