include Makefile.inc

SRC=src/
.PHONY: $(SRC) $(SRC)
.PHONY: clean
$(SRC):
	$(MAKE) -C $@

clean:
	$(MAKE) -C $(SRC) clean

force_look:
	true

