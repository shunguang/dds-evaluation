#!/bin/bash
make -f Makefile_util.mak -j4
make -f Makefile_msg.mak  -j4
make -f Makefile_cfg.mak -j4
make -f Makefile_fastPubEx1.mak -j4
make -f Makefile_fastSubEx1.mak -j4
make -f Makefile_test.mak -j4
