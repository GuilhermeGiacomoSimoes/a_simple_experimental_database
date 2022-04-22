all:install

SRC_PATH := ./src/
TEST_PATH := ./t/

compile_arm:  
	clang $(SRC_PATH)main.c $(SRC_PATH)b_tree.c $(SRC_PATH)disk_operation.c -g -arch arm64 -o REPL 

compile_x86:  
	clang $(SRC_PATH)main.c $(SRC_PATH)b_tree.c $(SRC_PATH)disk_operation.c -g -v -o REPL 

install: REPL 
	clang $(SRC_PATH)main.c $(SRC_PATH)b_tree.c $(SRC_PATH)disk_operation.c -o REPL 
	sudo mv $< /bin/

clean:
	rm -rf REPL REPL.dSYM database.db
