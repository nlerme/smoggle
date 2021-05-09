#####################################################################################
#                                                                                   #
#                       Compiling Makefile for documentation                        #
#                                                                                   #
#####################################################################################


#----------------------- Initialisation of misc. variables --------------------------

DOXYGEN_FILE = doc.doxygen
DOCS_DIRS    = `find . -maxdepth 1 -type d -printf "%f " | cut -d' ' -f2- -`


#---------------------------------- Makefile core -----------------------------------

all:
	@echo -n "+ Generate doc : "
	@doxygen $(DOXYGEN_FILE) 2>/dev/null >/dev/null
	@echo "Ok"

clean:
	@echo -n "+ Delete doc : "
	@for Dir in $(DOCS_DIRS); do \
		\rm -rf $$Dir/*; \
	done
	@echo "Ok"

.PHONY: all clean
