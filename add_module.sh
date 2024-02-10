#!/bin/bash

MODULE_NAME=/pam_simple.so
LOCAL_MODULE_PATH=`pwd`${MODULE_NAME}
GLOBAL_MODULE_PATH=/usr/lib/x86_64-linux-gnu/security${MODULE_NAME}

sudo rm ${GLOBAL_MODULE_PATH};
sudo cp ${LOCAL_MODULE_PATH} ${GLOBAL_MODULE_PATH};
sudo chmod -x ${GLOBAL_MODULE_PATH};
