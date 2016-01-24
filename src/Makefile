include ../Makefile.inc

DIRS=engine/ io/ tests/

.PHONY: all $(DIRS)
.PHONY: clean $(DIRS)

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@

clean:
	for dir in $(DIRS); do $(MAKE) $@ -C $$dir; done

