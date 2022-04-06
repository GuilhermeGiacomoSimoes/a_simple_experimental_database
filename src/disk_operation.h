#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
#define NAME_FILE_DATABASE "database.db"

const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);
const uint32_t ID_OFFSET = 0; 
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE; 
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE; 

void disk_write(Page* page);
Page* disk_read(Page* page, int number_child);
void serialize_row(Row* source, void* destination);
void deserialize_row(void *source, Row* destination);
