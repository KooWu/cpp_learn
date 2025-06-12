#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Import modules."""
import os
import json as js
from jinja2 import FileSystemLoader, Environment
import src.utils.util_func as util


class FdbusAppGen:
    """Generate application Class."""

    def __init__(self):
        loader = FileSystemLoader(".")
        self.env = Environment(loader=loader, trim_blocks=True, lstrip_blocks=True)
        self.env.filters["hex_to_decimal"] = util.hex_to_int
        self.env.filters["ns_prefix"] = util.ns_prefix_convert
        self.env.filters["ns_convert"] = util.ns_segemtation_convert
        self.env.filters["ns_colon"] = util.ns_colon_convert

    def __gen_app_file_dir(self, out_dir, app_name):
        file_dir = out_dir + "/" + app_name
        if not os.path.exists(file_dir):
            os.makedirs(file_dir)
        return file_dir

    def gen_app_files(self, out_dir, srv_if_json, deploy_json, target_app):
        """Generate application files."""
        for _, info in deploy_json.items():
            for app in info["apps"]:
                if target_app != "all" and target_app != app["name"]:
                    continue
                app_dir = self.__gen_app_file_dir(out_dir, app["name"])
                file_name = app_dir + "/" + app["name"] + "_main.cpp"
                with open(file_name, "w", encoding="utf-8") as f:
                    template = self.env.get_template(
                        "src/templates/app_main.cpp.j2"
                    )
                    result = template.render(app_info=app, srvs_info=srv_if_json)
                    f.write(result)

    def gen_app_cmake_file(self, out_dir, excel_name, deploy_json, target_app):
        """Generate Application CMakeLists.txt file."""
        for _, info in deploy_json.items():
            for app_info in info["apps"]:
                if target_app != "all" and target_app != app_info["name"]:
                    continue
                app_dir = self.__gen_app_file_dir(out_dir, app_info["name"])
                file_name = app_dir + "/CMakeLists.txt"
                with open(file_name, "w", encoding="utf-8") as f:
                    template = self.env.get_template(
                        "src/templates/app_cmake.txt.j2"
                    )
                    result = template.render(app_info=app_info, test_name=excel_name, app_name=app_info["name"])
                    f.write(result)
