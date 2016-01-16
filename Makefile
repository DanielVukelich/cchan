FILES=src/post.c src/thread.c src/board.c src/render.c
MAIN=main.c
TEST_MAIN=src/tests/main.c
OPTS=-Wall -Wextra -Werror
TEST_OPTS=-g
OUT=cchan
TEST_OUT=testcchan
LIBS=
all:
	gcc -o $(OUT) $(MAIN) $(FILES) $(OPTS) $(LIBS)
test:
	gcc -o $(TEST_OUT) $(TEST_MAIN) $(FILES) $(OPTS) $(TEST_OPTS) $(LIBS)
