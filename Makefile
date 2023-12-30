CC=gcc
BUILD_DIR=build
SRC_DIR=src
INC_DIR=include
INCLUDE=-I$(INC_DIR)
LIBS=-lncurses -lm
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=gnu17 -g -O0

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)
PROGS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))
#$(info $(PROGS))

all: $(PROGS)

$(BUILD_DIR)/Chronometer: $(SRC_DIR)/TimeUtils/TimeUtils.c $(SRC_DIR)/Chronometer.c

$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ $(LIBS)

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $?
	clang-tidy --fix $?

clean:
	rm -f $(PROGS)
	rm -f $(NOM_ZIP)

.PHONY: clean all
