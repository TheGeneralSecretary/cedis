MAKE_DIR = $(PWD)

SRC_DIR       := $(MAKE_DIR)/src
INCLUDE_DIR   := $(MAKE_DIR)/include
OBJ_DIR       := $(MAKE_DIR)/build/obj
LIB_DIR       := $(MAKE_DIR)/lib
BUILD_DIR	  := $(MAKE_DIR)/build
BIN_DIR		  := $(MAKE_DIR)/build/bin
EXTERNAL_DIR  := $(MAKE_DIR)/external

# External Libraries DIR
LIBCLOG_DIR	  := $(EXTERNAL_DIR)/libclog

INC_SRCH_PATH := 
INC_SRCH_PATH += -I$(INCLUDE_DIR)
INC_SRCH_PATH += -I$(LIBCLOG_DIR)

LIB_SRCH_PATH :=
LIB_SRCH_PATH += -L$(MAKE_DIR)/lib

CC = gcc
LD = ld

BIN = $(MAKE_DIR)/build/bin/cedis

LIBS :=

CFLAGS := -std=gnu99 -pedantic
CFLAGS += $(INC_SRCH_PATH) $(LIB_SRCH_PATH)

ifeq ($(BUILD),dev)
# "Dev" build - no optimization, and debugging symbols
CFLAGS += -g -ggdb3 -Og -DDEBUG -finstrument-functions -fdump-rtl-expand -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
LIBS   += -lclog-dev
else ifeq ($(BUILD),debug)
# "Debug" build - no optimization, and debugging symbols (for valgrind)
CFLAGS += -g -ggdb3 -Og -DDEBUG -finstrument-functions -fdump-rtl-expand -fno-omit-frame-pointer -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
LIBS   += -lclog-dev
else
# "Release" build - optimization, and no debug symbols
CFLAGS += -O2 -s -DNDEBUG -Wall -Wextra
LIBS   += -lclog
endif

LDFLAGS :=

export MAKE_DIR CC LD CFLAGS LDFLAGS LIBS INC_SRCH_PATH OBJ_DIR LIB_DIR BIN_DIR BIN LIBCLOG_DIR

all:
	@$(MAKE) -C $(EXTERNAL_DIR)
	@$(MAKE) -C $(SRC_DIR)

dev:
	make "BUILD=dev"

debug:
	make "BUILD=debug"

run:
	$(BIN)

valgrind:
	@mkdir -p build
	valgrind --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=./build/valgrind-out.txt \
        $(BIN) $(ARGS)

clangformat:
	find . -iname *.h -o -iname *.c | xargs clang-format -i

.PHONY: clean
clean:
	@$(MAKE) -C $(EXTERNAL_DIR) clean
	@$(MAKE) -C $(SRC_DIR)		clean

cleanall:
	@$(MAKE) -C $(EXTERNAL_DIR) clean
	@$(RM)	 -rf $(BUILD_DIR)
	@$(RM)	 -rf $(LIB_DIR)
