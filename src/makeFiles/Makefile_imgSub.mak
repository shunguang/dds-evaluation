-include Makefile.inc

PROJ_NAME=ImgSub

include Makefile_app_header.mak

#the target binary name
TARGETFILE=$(ODIR_BIN)/imgSub.out

# link libs
LIBS	:= -lMsg -lCfg -lUtil \
	-lfastrtps -lfastcdr \
	-lboost_timer -lboost_filesystem -lboost_system -lboost_date_time -lboost_regex \
	-lboost_chrono -lboost_thread -pthread \
	-lopencv_ml -lopencv_shape -lopencv_video -lopencv_calib3d -lopencv_features2d -lopencv_xfeatures2d \
	-lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_imgcodecs -lopencv_imgproc -lopencv_core \
	-lv4l2 \
	-ldl -lm -lpthread -lrt 

OBJS = \
	$(ODIR_OBJ)/ImgSub.o \
	$(ODIR_OBJ)/mainImgSub.o 

#	$(ODIR_OBJ)/test_px4_ekf.o \

default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_OBJ)
	mkdir -p $(ODIR_LIB)
	mkdir -p $(ODIR_BIN)
	
#the output binary file name is <$(TARGETFILE)>
$(TARGETFILE)	:	$(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) $(LIBS) $(LIBS) -o $(TARGETFILE)


$(ODIR_OBJ)/mainImgSub.o	:	$(SDIR_PROJ)/mainImgSub.cpp
	$(CXX) -o $(ODIR_OBJ)/mainImgSub.o $(CFLAGS_EXE) $(SDIR_PROJ)/mainImgSub.cpp

$(ODIR_OBJ)/ImgSub.o	:	$(SDIR_PROJ)/ImgSub.cpp
	$(CXX) -o $(ODIR_OBJ)/ImgSub.o $(CFLAGS_EXE) $(SDIR_PROJ)/ImgSub.cpp

clean:
	\rm $(ODIR_OBJ)/*.o $(TARGETFILE)

rm_imgSub:
	\rm $(TARGETFILE)
            
