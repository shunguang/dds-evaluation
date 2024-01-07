# DDS performance evaluation 
This version comes from the original one at https://github.com/shunguang/xZone.git

(08/16/2013)

## PGK and SRC dir
	(a) for windows
		PKG=c:\pkg
		XZONE_SRC=C:\Users\wus1\Projects\xZone\src
	(b) for linux
		PKG=~\pkg
		XZONE_SRC=~\xZone\src
	
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
	   1) copy $(XZONE_SRC)/vs2019/open_xZone_vs2019_template.bat to $(XZONE_SRC)/vs2019/open_xZone_vs2019_xyz.bat
	   2) edit  $(XZONE_SRC)/vs2019/open_xZone_vs2019_xyz.bat to make sure the path is corresponding to your envrionment
	   3) double click $(XZONE_SRC)/vs2019/open_xZone_vs2019_xyz.bat
	   
	b) for linux
	   1) set enviornment varaiables PKG and XZONE_SRC 
	   2) cd $(XZONE_SRC)/makeFiles execute 
	      ./run_all_w_cleanAll.sh 
	   	  (you may need to to edit Makefile_app_header.mak if there is any errors)	  
