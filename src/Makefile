include ../Makefile.inc

DIRS=engine io tests util net
OBJECTS=$(foreach dir, $(DIRS), $(wildcard $(dir)/*.o))

.PHONY: all $(DIRS)
.PHONY: clean $(DIRS)

all: $(TARGET)
	mv $(TARGET) ..

$(TARGET): $(DIRS) main.o
	$(CC) $(LDFLAGS) $(OBJECTS) main.o $(LDLIBS) -o $(TARGET)

main.o:
	$(CC) -o $@ -c main.c $(CFLAGS) -Iinclude/

$(DIRS):
	$(MAKE) -C $@

clean:
	for dir in $(DIRS); do $(MAKE) clean -C $$dir; done
	rm main.o

