SRC=src/post.c src/thread.c src/board.c src/render.c src/reader.c
OBJECTS=$(SRC:.c=.o)
MAIN=main.c
MAIN_OBJECT=$(MAIN:.c=.o)
CC=gcc
CFLAGS=-Werror -Wall -Wextra
LDFLAGS=
OUT=cchan
TEST_MAIN=src/tests/main.c
TEST_SRC=
TEST_OBJECTS=$(TEST_SRC:.c=.o)
TEST_MAINOBJ=$(TEST_MAIN:.c=.o)
TEST_LDFLAGS=-g
TEST_OUT=testcchan
ALL_OBJECTS=$(TEST_OBJECT) $(TEST_MAINOBJ) $(OBJECTS) $(MAIN_OBJECT)
ALL_OUT=$(TEST_OUT) $(OUT)

.PHONY:all
all: $(OUT) $(TEST_OUT)

$(OUT): $(OBJECTS) $(MAIN_OBJECT)
	$(CC) $(LDFLAGS) $(OBJECTS) $(MAIN_OBJECT) -o $@

$(TEST_OUT): $(OBJECTS) $(TEST_OBJECTS) $(TEST_MAINOBJ)
	$(CC) $(TEST_LDFLAGS) $(TEST_OBJECTS) $(OBJECTS) $(TEST_MAINOBJ) $(TEST_LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(ALL_OUT) $(ALL_OBJECTS) test*

.PHONY: clean

