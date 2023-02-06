#--- Variables --------------------------------------------------------------------------------------------
include Makefile.common

TARGET    = main
CXX       = g++
CXXFLAGS  = -O3 -Wall -g -fopenmp
LDFLAGS   = -L/usr/lib
INC_DIR   = -Iinclude -I/usr/include/libxml2
LIBS      = -lm -lxml2
SRC_DIR   = src
OBJ_DIR   = obj
DOC_DIR   = doc
FILES_EXT = cpp
SRC       = $(notdir $(wildcard $(SRC_DIR)/*.$(FILES_EXT)))
OBJ       = $(SRC:%.$(FILES_EXT)=$(OBJ_DIR)/%.o)

#--- Rules ------------------------------------------------------------------------------------------------
all: header depend $(OBJ) $(TARGET) footer

header:
	@echo "-----------------------------------------------------------------------------"
	@echo "Project name : $(PROJECT_NAME)"
	@echo "Description  : $(PROJECT_DESCRIPTION)"
	@echo "License      : $(PROJECT_LICENSE)"
	@echo "Version      : $(PROJECT_VERSION)"
	@echo "Authors      : $(PROJECT_AUTHORS)"
	@echo "Last release : $(PROJECT_LAST_RELEASE)"
	@echo -e "-----------------------------------------------------------------------------\n"

depend:
ifeq ("$(wildcard $(OBJ_DIR))","")
	@mkdir $(OBJ_DIR)
endif
	@echo -n "+ Generate dependencies : "
ifneq ($(strip $(SRC)),)
	@$(CXX) $(INC_DIR) -MM $(SRC:%=$(SRC_DIR)/%) \
	| sed -e 's:^[^ ]:$(OBJ_DIR)/&:' > Makefile.depend
endif
	@echo "OK"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(FILES_EXT)
	@echo -n "+ Compile $(notdir $@) : "
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INC_DIR) -c $< -o $@
	@echo "OK"

$(TARGET): $(OBJ)
	@echo -n "+ Build $@ : "
	@$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo "Ok"

clean: clean_doc
	@echo -n "+ Delete objects and bin : "
	@\rm -rf $(OBJ_DIR) $(TARGET)
	@echo "OK"

doc:
	@$(MAKE) -C $(DOC_DIR) -s

clean_doc:
	@$(MAKE) -C $(DOC_DIR) -s clean

footer:
	@echo -e "\n-----------------------------------------------------------------------------"

include Makefile.depend

.PHONY: doc clean clean_doc
