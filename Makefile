progs = $(patsubst map_%.c,map_%,$(wildcard map_*.c))
taras = $(patsubst map_%,taralli_%,$(progs))

all: $(taras)

CFLAGS += -std=c99
CFLAGS += -O9
CFLAGS += -Wall
CPPFLAGS += -DNDEBUG
LOADLIBES += -lX11 -lXi

$(progs): main.o

taralli_%: map_%
	ln -sf $< $@

clean:
	-rm -f $(progs) $(taras) main.o

.PHONY: all clean
