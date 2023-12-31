

all:
	clang -g frontend.c disk_operation.c service.c store.c -o db

