include Makefile.inc

SRC=src/
.PHONY: $(SRC) $(SRC)
.PHONY: clean

all: $(TARGET)

$(TARGET): $(SRC)

$(SRC):
	$(MAKE) -C $@

clean:
	$(MAKE) clean -C $(SRC)
	$(RM) $(TARGET)

force_look:
	true

