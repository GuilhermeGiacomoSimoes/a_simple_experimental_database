TARGET=a_simple_db
CXX=gcc
OBJS=REPL.c

a_simple_db:$(OBJS)
	$(LD) -o $(TARGET) $(OBJS)
install:
	@install a_simple_db /usr/local/bin/a_simple_db
clean:
	rm -rf *.o
