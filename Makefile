CFLAGS = -g
LDFLAGS = -Wl,-rpath=/home/drgrid/bitcoin/install_dir/lib
LIBS = -lbitcoinkernel

EXAMPLES_DIR = examples
EXAMPLES_SRC = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLES = $(EXAMPLES_SRC:$(EXAMPLES_DIR)/%.c=%)

.PHONY: all clean

all: $(EXAMPLES)

%: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

clean:
	rm -f $(EXAMPLES)
