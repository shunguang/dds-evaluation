-include Makefile.inc

PROJ_NAME=test

include Makefile_app_header.mak

#the target binary name
TARGETFILE=$(ODIR_BIN)/test.out

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
	$(ODIR_OBJ)/test_mat_part1.o \
	$(ODIR_OBJ)/test_main.o 

#	$(ODIR_OBJ)/test_px4_ekf.o \

default:  directories $(TARGETFILE)

directories:    
	mkdir -p $(ODIR_OBJ)
	mkdir -p $(ODIR_LIB)
	mkdir -p $(ODIR_BIN)
	
#the output binary file name is <$(TARGETFILE)>
$(TARGETFILE)	:	$(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) $(LIBS) $(LIBS) -o $(TARGETFILE)


$(ODIR_OBJ)/test_main.o	:	$(SDIR_PROJ)/test_main.cpp
	$(CXX) -o $(ODIR_OBJ)/test_main.o $(CFLAGS_EXE) $(SDIR_PROJ)/test_main.cpp

$(ODIR_OBJ)/test_mat_part1.o	:	$(SDIR_PROJ)/test_mat_part1.cpp
	$(CXX) -o $(ODIR_OBJ)/test_mat_part1.o $(CFLAGS_EXE) $(SDIR_PROJ)/test_mat_part1.cpp

clean:
	\rm $(ODIR_OBJ)/*.o $(TARGETFILE)

rm_pnt:
	\rm $(TARGETFILE)
            
