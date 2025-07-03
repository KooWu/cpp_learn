#!/bin/bash

# # sudo route add -net 224.244.224.245/4 dev enp0s8
# # if [ $# -gt 0 ];then
# route add -nv 224.244.224.245 dev eth0
# export VSOMEIP_CONFIGURATION=./sub_com.json
# # else
# # export VSOMEIP_CONFIGURATION=./sub_com.json
# # fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./../../lib/
export  VSOMEIP_APPLICATION_NAME=method_client
./soa_client