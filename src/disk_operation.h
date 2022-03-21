#define NAME_FILE_DATABASE "database.db";

void disk_write(Page* page);
Page* disk_read(Page* page, int number_child);
