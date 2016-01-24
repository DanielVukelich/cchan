include ../../Makefile.inc

SRC=reader.c render.c
OBJECTS=$(SRC:.c=.o)

.PHONY: all
all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

clean:
	$(RM) -f $(OBJECTS)

