# Variables
include Makefile.common

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

# Rules
all: header depend $(OBJ) $(TARGET) footer

header:
	@echo "-----------------------------------------------------------------------------"
	@echo "Project name : $(PROJECT_NAME)"
	@echo "Description  : $(DESCRIPTION)"
	@echo "Version      : $(VERSION)"
	@echo "Authors      : $(AUTHORS)"
	@echo "Website      : $(WEBSITE)"
	@echo "Last release : $(LAST_RELEASE)"
	@echo -e "-----------------------------------------------------------------------------\n"

depend:
	@echo -n "+ Generate dependencies : "
ifneq ($(strip $(SRC)),)
	@$(CXX) $(INC_DIR) -MM $(SRC:%=$(SRC_DIR)/%) \
	| sed -e 's:^[^ ]:$(OBJ_DIR)/&:' > Makefile.depend
endif
	@echo "Ok"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(FILES_EXT)
	@echo -n "+ Compile $(notdir $@) : "
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INC_DIR) -c $< -o $@
	@echo "Ok"

$(TARGET): $(OBJ)
	@echo -n "+ Build $@ : "
	@$(CXX) $(CXXFLAGS) $(INC_DIR) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo "Ok"

clean: clean_doc
	@echo -n "+ Delete objects and bin : "
	@\rm -f $(OBJ_DIR)/*.o $(TARGET)
	@echo "Ok"

doc:
	@$(MAKE) -C $(DOC_DIR) -s

clean_doc:
	@$(MAKE) -C $(DOC_DIR) -s clean

footer:
	@echo -e "\n-----------------------------------------------------------------------------"

include Makefile.depend

.PHONY: doc clean clean_doc
