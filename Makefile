all:install

SRC_PATH := ./src/
TEST_PATH := ./t/

compile:  
	clang $(SRC_PATH)main.c $(SRC_PATH)b_tree.c $(SRC_PATH)disk_operation.c -o REPL 

install: REPL 
	clang $(SRC_PATH)main.c $(SRC_PATH)b_tree.c $(SRC_PATH)disk_operation.c -o REPL 
	sudo mv $< /bin/

clean:
	$(RM) REPL
