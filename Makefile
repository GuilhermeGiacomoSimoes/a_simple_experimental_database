all:install

SRC_PATH := ./src/
TEST_PATH := ./t/

ggs: $(SRC_PATH)REPL.c
	gcc $(SRC_PATH)REPL.c -o $@ 

test: $(SRC_PATH)REPL.c
	bundle exec rspec

install: test ggs 
	sudo mv $< /bin/

clean:
	rm -rf ggs db.db
