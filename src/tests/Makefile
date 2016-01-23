include ../../Makefile.inc

SRC=main.c
OBJECTS=$(SRC:.c=.o)

.PHONY: all
all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

