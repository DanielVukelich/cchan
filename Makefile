include Makefile.inc

SRC=src/
.PHONY: $(SRC) $(SRC)
.PHONY: clean
$(SRC):
	$(MAKE) -C $@

clean:
	$(MAKE) clean -C $(SRC)

force_look:
	true

