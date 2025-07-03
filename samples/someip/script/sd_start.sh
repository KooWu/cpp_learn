#!/bin/bash
# 1 local 20 2000 1024 0
# 1 local 20 2000 4096 0
# route add -nv 224.244.224.245 dev eth0
if [ $# -gt 0 ];then
export VSOMEIP_CONFIGURATION=./sd_server.json
else
export VSOMEIP_CONFIGURATION=./sd_client.json
fi
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./../../lib/
export  VSOMEIP_APPLICATION_NAME=routingmanagerd
./../../bin/routingmanagerd
