#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Import modules."""
import os
import logging
from src.gen.convert_fdbus_json import ConverterFdbusJson
from src.gen.convert_proto_json import ConverterProtoJson
from src.gen.fdbus_mw_gen import FdbusMwGen
from src.gen.fdbus_app_gen import FdbusAppGen

def fdbus_gen_entry(args, data_type_json, srv_if_json, deploy_json):
    """The entry function of fdbus gen."""
    logging.info("Start to generate fdbus files.")

    excel_name = os.path.basename(args.xlsx)
    file_name, _ = os.path.splitext(excel_name)

    # 转换json数据格式
    convert_proto_json = ConverterProtoJson()
    proto_data_type_json = convert_proto_json.convert_data_type_json(data_type_json)
    print(proto_data_type_json)
    convert_fdbus_json = ConverterFdbusJson()
    fdbus_srv_if_json = convert_fdbus_json.convert_srv_if_json(srv_if_json)
    # print(fdbus_srv_if_json)
    fdbus_mw_gen = FdbusMwGen()
    fdbus_mw_gen.gen_fdbus_mw_files(
        args.output, args.pb_version, proto_data_type_json, fdbus_srv_if_json, deploy_json, args.target_app
    )
    # 生成实例工程文件
    if args.sample:
        app_gen = FdbusAppGen()
        app_gen.gen_app_files(args.output, fdbus_srv_if_json, deploy_json, args.target_app)
        app_gen.gen_app_cmake_file(args.output, file_name, deploy_json, args.target_app)
    logging.info("Finish generating fdbus files.")
