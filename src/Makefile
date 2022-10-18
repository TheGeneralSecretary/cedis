SRCS = $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(BIN): $(SRCS)
	@mkdir -p $(BIN_DIR)
	@$(CC) $^ $(CFLAGS) -Wl,-Map=$(BIN).map $(LIBS) -o $@
	@echo "    Generate Program $(notdir $(BIN)) from $^"

.PHONY: clean
clean:
	@$(RM) -f $(OBJS) $(BIN)
	@$(RM) -f *.expand
	@$(RM) -rf $(BIN_DIR)
	@echo "    Remove Objects:   $(OBJS)"
	@echo "    Remove Libraries:  $(notdir $(BIN))"
