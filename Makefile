include Makefile.inc

SRC=src/
.PHONY: $(SRC) $(SRC)
$(SRC):
	$(MAKE) -C $@

force_look:
	true

