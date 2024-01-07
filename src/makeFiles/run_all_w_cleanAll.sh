#!/bin/bash

#clean all
make -f Makefile_util.mak clean
make -f Makefile_msg.mak clean
make -f Makefile_cfg.mak clean
make -f Makefile_imgPub.mak clean
make -f Makefile_imgSub.mak clean
make -f Makefile_fastPubEx1.mak  clean
make -f Makefile_fastSubEx1.mak  clean
make -f Makefile_test.mak  clean

#---------------------------------------

make -f Makefile_util.mak -j4
make -f Makefile_msg.mak  -j4
make -f Makefile_cfg.mak -j4
make -f Makefile_imgPub.mak -j4
make -f Makefile_imgSub.mak -j4
make -f Makefile_fastPubEx1.mak -j4
make -f Makefile_fastSubEx1.mak -j4
make -f Makefile_test.mak -j4
#---eof---

