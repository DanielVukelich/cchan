SRC=main.c src/post.c src/thread.c src/board.c src/render.c src/reader.c
OBJECTS=$(SRC:.c=.o)
CC=gcc
CFLAGS=-Werror -Wall -Wextra
LDFLAGS=
OUT=cchan

.PHONY:all
all: cchan

$(OUT): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OUT) $(OBJECTS)

.PHONY: clean

