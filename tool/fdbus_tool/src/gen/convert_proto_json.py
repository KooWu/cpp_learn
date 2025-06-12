#!/usr/bin/env python3

import src.utils.util_func as util
from jinja2 import FileSystemLoader, Environment


class ConverterProtoJson:
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
            "int8": "sint32",
            "int16": "sint32",
            "int32": "sint32",
            "int64": "sint64",
            "uint8": "uint32",
            "uint16": "uint32",
            "uint32": "uint32",
            "uint64": "uint64",
            "string": "string",
        }

    def __excel_type2proto_type(self, data_type_json):
        for _, info in data_type_json.items():
            msg_count = len(info["messages"])
            for i in range(msg_count):
                field_len = len(info["messages"][i]["members"])
                for j in range(field_len):
                    info["messages"][i]["members"][j]["is_repeated"] = False
                    info["messages"][i]["members"][j]["idx"] = j + 1
                    type = info["messages"][i]["members"][j]["type"]
                    if type in self.std_types_map:
                        type = self.std_types_map[type]
                    elif util.is_array_type(type):
                        type, arr_size = util.get_array_type(type)
                        if type == "uint8":
                            type = "bytes"
                        elif type in self.std_types_map:
                            type = self.std_types_map[type]
                        info["messages"][i]["members"][j]["is_repeated"] = True
                    elif util.is_map_type(type):
                        type1, type2 = util.get_map_type(type)
                        print(type1, type2)
                        if type1 in self.std_types_map:
                            type1 = self.std_types_map[type1]
                        if type2 in self.std_types_map:
                            type2 = self.std_types_map[type2]
                        type = "map<{},{}>".format(type1, type2)
                    info["messages"][i]["members"][j]["type"] = type
                

    def convert_data_type_json(self, data_type_json):
        """Convert data type json from excel to fidl data type."""
        json = data_type_json.copy()
        self.__excel_type2proto_type(json)
        return json
