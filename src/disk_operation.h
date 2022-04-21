#ifndef _DISK_OPERATION_H_ 
#define _DISK_OPERATION_H_ 

#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define NAME_FILE_DATABASE "database.db"
#define ID_SIZE size_of_attribute(Row, id)
#define USERNAME_SIZE size_of_attribute(Row, username)
#define EMAIL_SIZE size_of_attribute(Row, email)
#define ID_OFFSET 0
#define USERNAME_OFFSET ID_OFFSET + ID_SIZE
#define EMAIL_OFFSET USERNAME_OFFSET + USERNAME_SIZE

void disk_write(Page* page);
Page* disk_read(Page* page, int number_child);
void serialize_row(Row* source, void* destination);
void deserialize_row(void *source, Row* destination);

#endif
