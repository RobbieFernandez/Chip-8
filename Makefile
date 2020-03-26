OUT_DIR = out
INCLUDE_DIR = include
SRC_DIR = src

_DEPS = chip8.h
DEPS = $(patsubst %,$(INCLUDE_DIR)/%,$(_DEPS))

_OBJECTS = main.o chip8.o
OBJECTS = $(patsubst %,$(OUT_DIR)/%,$(_OBJECTS))

CC = g++
OUT = $(OUT_DIR)/chip8
CFLAGS = -I$(INCLUDE_DIR)

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) $(OUT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT): $(OBJECTS)
	$(CC) -o $(OUT) $(OBJECTS)

.PHONY: build clean

build: $(OUT)

clean:
	rm -r $(OUT_DIR)