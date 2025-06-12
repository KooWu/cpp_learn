#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Import argparse for parameter parsing."""
import argparse
import logging
from src.utils.log_config import init_logging
from src.utils.excel_parse import ExeclParser
from src.gen.fdbus_entry import fdbus_gen_entry

def fdbus_gen(args, data_type_json, srv_if_json, deploy_json):
    """The entry function of fidl generator."""
    fdbus_gen_entry(args, data_type_json, srv_if_json, deploy_json)

def add_common_argument(parser):
    """Add common argument to the given parser object."""
    parser.add_argument(
        "-x",
        "--xlsx",
        type=str,
        help="execl that need to be converted (default: None)",
        default=None,
    )

    parser.add_argument(
        "-o",
        "--output",
        type=str,
        help="output file dir (default: output)",
        default="output",
    )

    parser.add_argument(
        "--debug", type=bool, help="debug mode (default: True)", default=True,
    )

    parser.add_argument(
        "--pb_version", type=int, help="protobuf version (default: 2)", default=2,
    )

def check_args(args):
    """Check the validity of the arguments."""
    if args.xlsx is None or args.output is None:
        raise ValueError("xlsx file is not specified")
    logging.info('xlsx: %s', args.xlsx)
    logging.info('output: %s', args.output)

def excel2json(args, data_type_json, srv_if_json, deploy_json):
    """parse the excel file and generate the json files."""
    parser = ExeclParser(args.xlsx)
    parser.gen_data_type_json("Data Type", data_type_json)
    parser.gen_srv_if_json("Service Interface", srv_if_json)
    parser.gen_deploy_json("Service Deployment", deploy_json)
    parser.close_excel()

def add_subparser(subparsers):
    """Add a subparser to the given subparsers object."""
    # 添加 fidl代码生成 子命令
    parser_cmd = subparsers.add_parser("fdbus_gen", help="run fdbus_gen")
    add_common_argument(parser_cmd)
    parser_cmd.add_argument(
        "--sample", type=bool, help="Generate sample project or not", default=False,
    )

    parser_cmd.add_argument(
        "--target_app", type=str, help="target app (default: all)", default="all",
    )
    parser_cmd.set_defaults(func=fdbus_gen)

def main():
    """The entry function of excel2x tool."""
    init_logging()

    # 创建顶级解析器
    parser = argparse.ArgumentParser(description="excel2x tool with subcommands")

    # 添加子命令
    subparsers = parser.add_subparsers(title="Commands", dest="command")
    add_subparser(subparsers)

    # 解析命令行参数
    args = parser.parse_args()
    check_args(args)

    # 解析excel文件转json文件
    data_type_json = {}
    srv_if_json = {}
    deploy_json = {}
    excel2json(args, data_type_json, srv_if_json, deploy_json)
    if args.debug:
        print("data_type_json")
        print(data_type_json)
        print("srv_if_json")
        print(srv_if_json)
        print("deploy_json")
        print(deploy_json)

    # 调用子命令的函数
    if hasattr(args, "func"):
        args.func(args, data_type_json, srv_if_json, deploy_json)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
