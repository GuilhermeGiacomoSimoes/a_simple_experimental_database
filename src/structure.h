#define MAX_ELEMENTS 1000
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100

typedef struct {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE + 1];
	char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
	int folha;
	int elems;
	int current_address_memmory;
	struct Row* info[MAX_ELEMENTS];
	int childs[MAX_ELEMENTS + 1];
} Page;
