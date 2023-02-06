#--- Variables --------------------------------------------------------------------------------------------
DOXYGEN_FILE = doc.doxygen
DOCS_DIRS    = `find . -maxdepth 1 -type d -printf "%f " | cut -d' ' -f2- -`

#--- Rules ------------------------------------------------------------------------------------------------
all:
	@echo -n "+ Generate doc : "
	@doxygen $(DOXYGEN_FILE) 2>/dev/null >/dev/null
	@echo "OK"

clean:
	@echo -n "+ Delete doc : "
	@for Dir in $(DOCS_DIRS); do \
		\rm -rf $$Dir/*; \
	done
	@echo "OK"

.PHONY: all clean
