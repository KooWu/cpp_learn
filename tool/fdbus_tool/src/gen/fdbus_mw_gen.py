#!/usr/bin/env python3

"""Import modules."""
import os
from jinja2 import FileSystemLoader, Environment
import src.utils.util_func as util


class FdbusMwGen:
    """Generate Fdbus Middleware Class."""

    def __init__(self):
        loader = FileSystemLoader(".")
        self.env = Environment(loader=loader, trim_blocks=True, lstrip_blocks=True)
        self.env.filters["lower_first"] = util.lower_first
        self.env.filters["ns_prefix"] = util.ns_prefix_convert
        self.env.filters["ns_convert"] = util.ns_segemtation_convert
        self.env.filters["byte_type"] = util.get_byte_type

    def __gen_fdbus_gen_file_dir(self, out_dir, app_name):
        file_dir = out_dir + "/" + app_name + "/fdbus_gen/"
        if not os.path.exists(file_dir):
            os.makedirs(file_dir)
        return file_dir

    def __gen_proto_gen_file_dir(self, out_dir, app_name, pb_version):
        file_dir = out_dir + "/" + app_name + "/proto_gen/proto2"
        if pb_version == 3:
            file_dir = out_dir + "/" + app_name + "/proto_gen/proto3"
        if not os.path.exists(file_dir):
            os.makedirs(file_dir)
        return file_dir

    def __check_app_is_need_the_srv(self, srv_name, app_info):
        found = False
        for item in app_info["services"]:
            if 'srv_name' in item and item['srv_name'] == srv_name:
                found = True
                break
        return found

    def __check_app_is_need_the_req_srv(self, srv_name, app_info):
        found = False
        for item in app_info["req_services"]:
            if 'srv_name' in item and item['srv_name'] == srv_name:
                found = True
                break
        return found

    def __check_app_is_need_the_all_srv(self, srv_name, app_info):
        found = False
        for item in app_info["req_services"]:
            if 'srv_name' in item and item['srv_name'] == srv_name:
                found = True
                break
        for item in app_info["services"]:
            if 'srv_name' in item and item['srv_name'] == srv_name:
                found = True
                break
        return found

    def __gen_srv_if_client_file(self, out_dir, srv_if_json, app_info):
        app_dir = self.__gen_fdbus_gen_file_dir(out_dir, app_info["name"])
        for name, info in srv_if_json.items():
            if not self.__check_app_is_need_the_req_srv(name, app_info):
                continue
            file_name = app_dir + "/client_" + name.lower() + ".h"
            with open(file_name, "w", encoding="utf-8") as f:
                template = self.env.get_template(
                    "src/templates/fdbus_gen/client_srv.h.j2"
                )
                result = template.render(
                    ns=info["ns"], srv_name=name, srv_info=info
                )
                f.write(result)

            file_name = app_dir + "/client_" + name.lower() + ".cpp"
            with open(file_name, "w", encoding="utf-8") as f:
                template = self.env.get_template(
                    "src/templates/fdbus_gen/client_srv.cpp.j2"
                )
                result = template.render(
                    ns=info["ns"], srv_name=name, srv_info=info
                )
                f.write(result)

    def __gen_srv_if_server_file(self, out_dir, srv_if_json, app_info):
        app_dir = self.__gen_fdbus_gen_file_dir(out_dir, app_info["name"])
        for name, info in srv_if_json.items():
            if not self.__check_app_is_need_the_srv(name, app_info):
                continue
            file_name = app_dir + "/server_" + name.lower() + ".h"
            with open(file_name, "w", encoding="utf-8") as f:
                template = self.env.get_template(
                    "src/templates/fdbus_gen/server_srv.h.j2"
                )
                result = template.render(
                    ns=info["ns"], srv_name=name, srv_info=info
                )
                f.write(result)
            file_name = app_dir + "/server_" + name.lower() + ".cpp"
            with open(file_name, "w", encoding="utf-8") as f:
                template = self.env.get_template(
                    "src/templates/fdbus_gen/server_srv.cpp.j2"
                )
                result = template.render(
                   ns=info["ns"], srv_name=name, srv_info=info
                )
                f.write(result)

    def __gen_srv_if_proto_file(self, out_dir, pb_version, proto_data_type_json, srv_if_json, app_info):
        app_dir = self.__gen_proto_gen_file_dir(out_dir, app_info["name"], pb_version)
        bp_template_file = "src/templates/proto_gen/srv.proto2.j2"
        if pb_version == 3:
            bp_template_file = "src/templates/proto_gen/srv.proto.j2"
        for name, info in srv_if_json.items():
            if not self.__check_app_is_need_the_all_srv(name, app_info):
                continue
            file_name = app_dir + "/" + name.lower() + ".proto"
            for data_srv_name, data_infos in proto_data_type_json.items():
                if data_srv_name != name:
                    continue
                with open(file_name, "w", encoding="utf-8") as f:
                    template = self.env.get_template(
                        bp_template_file
                    )
                    result = template.render(
                        ns=data_infos["ns"], infos=data_infos["messages"]
                    )
                    f.write(result)


    def gen_fdbus_mw_files(self, out_dir, pb_version, proto_data_type_json, srv_if_json, deploy_json, target_app):
        """Generate fdbus mw files."""
        for _, info in deploy_json.items():
            for app_info in info["apps"]:
                if target_app != "all" and target_app != app_info["name"]:
                    continue
                self.__gen_srv_if_client_file(out_dir, srv_if_json, app_info)
                self.__gen_srv_if_server_file(out_dir, srv_if_json, app_info)
                self.__gen_srv_if_proto_file(out_dir, pb_version, proto_data_type_json, srv_if_json, app_info)

