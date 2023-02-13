CC=gcc
BUILD_DIR=build
SRC_DIR=src
INC_DIR=include
INCLUDE=-I$(INC_DIR)
LIBS=-lncurses -lm
CFLAGS=-fdiagnostics-color=always -Wall -pedantic -std=gnu17 -g -O0
NOM_ZIP=TP2_SEBAN_POUYANFAR.zip

SOURCES=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(wildcard $(INC_DIR)/*.h)
PROGS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(SOURCES))
CONTENU_ZIP=$(SRC_DIR) .clang-format .clang-tidy Makefile rapport.pdf
#$(info $(PROGS))

all: $(PROGS)

$(BUILD_DIR)/ChronoSimple: $(SRC_DIR)/TimeUtils/TimeUtils.c $(SRC_DIR)/ChronoSimple.c

$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir --parents $(BUILD_DIR)
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@ $(LIBS)

rapport: rapport.pdf

rapport.pdf: rapport.md
	@mkdir --parents logos
	@wget --quiet --show-progress --no-clobber -O logos/LogoLIGM.png "https://drive.google.com/uc?export=download&confirm=yes&id=1cZjxS6Rwp8LU4Eyahqz0eUS8aH0_VrVB" || true
	@wget --quiet --show-progress --no-clobber -O logos/namedlogoUGE.png "https://drive.google.com/uc?export=download&confirm=yes&id=1YGm1N7griuDbJhC6rSgBHrrcOsHKM5xg" || true
	pandoc --toc rapport.md -o rapport.pdf

format: $(SOURCES) $(HEADERS)
	clang-format -i --style=file $?
	clang-tidy --fix $?

clean:
	rm -f $(PROGS)
	rm -f $(NOM_ZIP)

zip:
	zip -FSr $(NOM_ZIP) $(CONTENU_ZIP)

rendu:
	@$(MAKE) --no-print-directory rapport
	@$(MAKE) --no-print-directory zip

.PHONY: clean all
