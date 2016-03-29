CC=gcc
OUT=cchan
TEST_OUT=testcchan
MAIN_SRC=main.c
RM=rm
CFLAGS=-Wall -Wextra -Werror -g -pedantic -ansi -D_POSIX_C_SOURCE=200112L -fstack-protector-strong
INCLUDE_DIR=../include
TARGET=cchan
LD=gcc
LDFLAGS=
LDLIBS=-lcrypto
