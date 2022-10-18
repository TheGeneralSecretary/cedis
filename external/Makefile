unexport MAKE_DIR CC LD CFLAGS LDFLAGS LIBS INC_SRCH_PATH OBJ_DIR LIB_DIR BIN_DIR BIN

all: lclog

lclog:
	@mkdir -p $(LIB_DIR)
	@$(MAKE) -C $(LIBCLOG_DIR) BUILD=""
	cp $(LIBCLOG_DIR)/lib/libclog.a $(LIB_DIR)/
	@$(MAKE) -C $(LIBCLOG_DIR) dev
	cp $(LIBCLOG_DIR)/lib/libclog-dev.a $(LIB_DIR)/

.PHONY: clean
clean:
	@$(MAKE) -C $(LIBCLOG_DIR) clean
	@echo "Remove Deps"
