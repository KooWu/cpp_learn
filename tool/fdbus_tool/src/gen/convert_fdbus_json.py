#!/usr/bin/env python3

import src.utils.util_func as util
from jinja2 import FileSystemLoader, Environment


class ConverterFdbusJson:
    """
    转换excel数据类型到fidl数据类型
    """

    def __init__(self):
        loader = FileSystemLoader(".")
        self.env = Environment(loader=loader, trim_blocks=True, lstrip_blocks=True)

        # 类型转换映射表
        self.std_types_map = {
            "bool": "bool",
            "float": "float",
            "double": "double",
            "int8": "int8_t",
            "int16": "int16_t",
            "int32": "int32_t",
            "int64": "int64_t",
            "uint8": "uint8_t",
            "uint16": "uint16_t",
            "uint32": "uint32_t",
            "uint64": "uint64_t",
            "string": "std::string",
        }

    def convert_srv_if_json(self, srv_if_json):
        """Convert service interface json from excel to fidl service interface."""
        for _, info in srv_if_json.items():
            length = len(info["interfaces"])
            info["ns"] = info["ns"].split(".")
            for i in range(length):
                if "out" in info["interfaces"][i]["param"]:
                    if "type" in info["interfaces"][i]["param"]["out"]:
                        type = info["interfaces"][i]["param"]["out"]["type"]
                        info["interfaces"][i]["param"]["out"]["is_basic"] = False
                        if type in self.std_types_map:
                            type = self.std_types_map[type]
                            info["interfaces"][i]["param"]["out"]["is_basic"] = True
                        info["interfaces"][i]["param"]["out"]["type"] = type
                if "in" in info["interfaces"][i]["param"]:
                    if "type" in info["interfaces"][i]["param"]["in"]:
                        info["interfaces"][i]["param"]["in"]["is_basic"] = False
                        type = info["interfaces"][i]["param"]["in"]["type"]
                        if type in self.std_types_map:
                            type = self.std_types_map[type]
                            info["interfaces"][i]["param"]["in"]["is_basic"] = True
                        info["interfaces"][i]["param"]["in"]["type"] = type
        return srv_if_json
