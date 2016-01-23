include ../../Makefile.inc

SRC=thread.c board.c post.c
OBJECTS=$(SRC:.c=.o)

.PHONY: all
.PHONY: clean
all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

clean:
	-rm $(OBJECTS)

