#!/bin/bash

idl_file_path=/home/yaoxingwei0382/my/cpp_learn/samples/someip/idl
idl_gen_file_path=/home/yaoxingwei0382/my/cpp_learn/samples/someip/src_gen

idl_core_gen=/home/yaoxingwei0382/my/cpp_learn/tool/commonapi_core_gen
idl_someip_gen=/home/yaoxingwei0382/my/cpp_learn/tool/commonapi_someip_gen

cd ${idl_core_gen}
./commonapi-core-generator-linux-x86_64 ${idl_file_path}/someip_desc_types.fidl -d ${idl_gen_file_path}/
./commonapi-core-generator-linux-x86_64 -sk ${idl_file_path}/someip_desc_services.fidl -d ${idl_gen_file_path}/

cd ${idl_someip_gen}
./commonapi-someip-generator-linux-x86_64 ${idl_file_path}/*.fdepl -d ${idl_gen_file_path}/