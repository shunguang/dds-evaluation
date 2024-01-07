# Build DDS performance evaluation 
(01/06/2014)

## PGK folder
	(a) for windows
		PKG=c:/pkg
	(b) for linux
		PKG=~/pkg
		
## DDS_EVAL folder
We assume DDS_EVAL is the check out folder of this project.

	(a) for windows
		DDS_EVAL=C:/Users/wus1/Projects/dds-evaluation/
	(b) for linux
		DDS_EVAL=~/dds-evaluation/src
	
## Dependency
    a) gtest
      GTEST_INC=$(PKG)\googletest\vs2019-install\include
	  GTEST_LIB=$(PKG)\googletest\vs2019-install\lib
	  
    b) opencv
		CV_INC=$(PKG)\opencv\opencv-4.1.1\install-local-w-contrib-vc2019-x64\include
		CV_LIB=$(PKG)\opencv\opencv-4.1.1\install-local-w-contrib-vc2019-x64\x64\vc16\lib
	
	c) boost
		BOOST_INC=$(PKG)\boost\boost_1_71_0
		BOOST_LIB=$(PKG)\boost\boost_1_71_0\lib64-msvc-14.2
		
	d) fast DDS
		DDS_INC=$(PKG)\fastDDS\v2.9.1\include
		DDS_LIB=$(PKG)\fastDDS\v2.9.1\lib\x64Win64VS2019
	
## How to build 
	a) for windows (vsiual studio 2019 and uper)
	   1) copy $(DDS_EVAL)/src/vs2019/open_xZone_vs2019_template.bat to $(DDS_EVAL)/src/vs2019/open_xZone_vs2019_xyz.bat
	   2) edit  $(DDS_EVAL)/src/vs2019/open_xZone_vs2019_xyz.bat to make sure the path is corresponding to your envrionment
	   3) double click $(DDS_EVAL)/src/vs2019/open_xZone_vs2019_xyz.bat
	   
	b) for linux
	   1) set enviornment varaiables PKG and DDS_EVAL_SRC 
	   2) cd $(DDS_EVAL)/src/makeFiles execute 
	      ./run_all_w_cleanAll.sh 
	   	  (you may need to to edit Makefile_app_header.mak if there is any errors)	  

## Results
see screen shot in folder doc/
 
