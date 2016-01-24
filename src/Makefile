include ../Makefile.inc

DIRS=engine io tests
OBJECTS=$(foreach dir, $(DIRS), $(wildcard $(dir)/*.o))

.PHONY: all $(DIRS)
.PHONY: clean $(DIRS)

all: $(TARGET)
	mv $(TARGET) ..

$(TARGET): $(DIRS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

$(DIRS):
	$(MAKE) -C $@

clean:
	for dir in $(DIRS); do $(MAKE) clean -C $$dir; done

