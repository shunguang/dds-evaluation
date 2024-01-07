#this Makefile outputs image as a static lib for <PLTF> machine

PROJ_NAME=libUtil
include Makefile_app_header.mak

OBJS = \
	$(ODIR_OBJ)/AppBagRead.o \
	$(ODIR_OBJ)/AppBagWrite.o \
	$(ODIR_OBJ)/AppLog.o \
	$(ODIR_OBJ)/CRC.o \
	$(ODIR_OBJ)/CvUtilFuncs.o \
	$(ODIR_OBJ)/FileUtil.o \
	$(ODIR_OBJ)/Roi.o \
	$(ODIR_OBJ)/HostYuvFrm.o \
	$(ODIR_OBJ)/Uint8Stream.o \
	$(ODIR_OBJ)/UtilFuncs.o 
	
default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_OBJ)
	mkdir -p $(ODIR_LIB)

#the output lib file name is <$(TARGETFILE)>
$(TARGETFILE) : $(OBJS)
	ar rcs $(TARGETFILE) $(OBJS)

$(ODIR_OBJ)/AppBagRead.o: $(SDIR_PROJ)/AppBagRead.cpp $(SDIR_PROJ)/AppBagRead.h
	$(CXX) -o $(ODIR_OBJ)/AppBagRead.o $(CFLAGS) $(SDIR_PROJ)/AppBagRead.cpp

$(ODIR_OBJ)/AppBagWrite.o: $(SDIR_PROJ)/AppBagWrite.cpp $(SDIR_PROJ)/AppBagWrite.h
	$(CXX) -o $(ODIR_OBJ)/AppBagWrite.o $(CFLAGS) $(SDIR_PROJ)/AppBagWrite.cpp

$(ODIR_OBJ)/AppLog.o: $(SDIR_PROJ)/AppLog.cpp $(SDIR_PROJ)/AppLog.h
	$(CXX) -o $(ODIR_OBJ)/AppLog.o $(CFLAGS) $(SDIR_PROJ)/AppLog.cpp

$(ODIR_OBJ)/CRC.o: $(SDIR_PROJ)/CRC.cpp $(SDIR_PROJ)/CRC.h
	$(CXX) -o $(ODIR_OBJ)/CRC.o $(CFLAGS) $(SDIR_PROJ)/CRC.cpp

$(ODIR_OBJ)/CvUtilFuncs.o: $(SDIR_PROJ)/CvUtilFuncs.cpp $(SDIR_PROJ)/CvUtilFuncs.h
	$(CXX) -o $(ODIR_OBJ)/CvUtilFuncs.o $(CFLAGS) $(SDIR_PROJ)/CvUtilFuncs.cpp

$(ODIR_OBJ)/FileUtil.o: $(SDIR_PROJ)/FileUtil.cpp $(SDIR_PROJ)/FileUtil.h
	$(CXX) -o $(ODIR_OBJ)/FileUtil.o $(CFLAGS) $(SDIR_PROJ)/FileUtil.cpp

$(ODIR_OBJ)/Roi.o: $(SDIR_PROJ)/Roi.cpp $(SDIR_PROJ)/Roi.h
	$(CXX) -o $(ODIR_OBJ)/Roi.o $(CFLAGS) $(SDIR_PROJ)/Roi.cpp

$(ODIR_OBJ)/HostYuvFrm.o: $(SDIR_PROJ)/HostYuvFrm.cpp $(SDIR_PROJ)/HostYuvFrm.h
	$(CXX) -o $(ODIR_OBJ)/HostYuvFrm.o $(CFLAGS) $(SDIR_PROJ)/HostYuvFrm.cpp

$(ODIR_OBJ)/Uint8Stream.o: $(SDIR_PROJ)/Uint8Stream.cpp $(SDIR_PROJ)/Uint8Stream.h
	$(CXX) -o $(ODIR_OBJ)/Uint8Stream.o $(CFLAGS) $(SDIR_PROJ)/Uint8Stream.cpp

$(ODIR_OBJ)/UtilFuncs.o: $(SDIR_PROJ)/UtilFuncs.cpp $(SDIR_PROJ)/UtilFuncs.h
	$(CXX) -o $(ODIR_OBJ)/UtilFuncs.o $(CFLAGS) $(SDIR_PROJ)/UtilFuncs.cpp

clean:
	\rm -r $(ODIR_OBJ)/*.o $(TARGETFILE)

