#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libMsg
include Makefile_app_header.mak

# cpp files
SRC_FILES1 := $(wildcard $(SDIR_PROJ)/*.cpp)
OBJ_FILES1 := $(patsubst $(SDIR_PROJ)/%.cpp,$(ODIR_OBJ)/%.o,$(SRC_FILES1))

# cxx files
SRC_FILES2 := $(wildcard $(SDIR_PROJ)/*.cxx)
OBJ_FILES2 := $(patsubst $(SDIR_PROJ)/%.cxx,$(ODIR_OBJ)/%.o,$(SRC_FILES2))

OBJS = \
	$(OBJ_FILES1)\
	$(OBJ_FILES2)\


default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_OBJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)

$(ODIR_OBJ)/%.o: $(SDIR_PROJ)/%.cxx
	$(CXX) $(CFLAGS) -c -o  $@ $<

$(ODIR_OBJ)/%.o: $(SDIR_PROJ)/%.cpp
	$(CXX) $(CFLAGS) -c -o  $@ $<

clean:
	\rm -r $(ODIR_OBJ)/*.o $(TARGETFILE)

