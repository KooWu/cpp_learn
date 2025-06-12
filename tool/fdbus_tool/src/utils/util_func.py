#!/usr/bin/env python3

"""Import modules."""
import re
import hashlib


def is_array_type(data_type):
    """Check if the data type is an array type."""
    if not isinstance(data_type, (str, bytes)):
        return False  # 或者抛出异常，取决于您的需求
    return re.search(r"([\S]+)\[([\d]*)\]", data_type) is not None


def get_array_type(data_type):
    """Get the array type and size."""
    res = re.search(r"([\S]+)\[([\d]*)\]", data_type)
    assert res is not None
    return res.group(1), res.group(2)


def is_two_dim_array_type(data_type):
    """Check if the data type is a two-dimensional array type."""
    return re.search(r"([\S]+)\[([\d]*)\]\[([\d]*)\]", data_type) is not None


def get_two_dim_array_type(data_type):
    """Get the two-dimensional array type and sizes."""
    res = re.search(r"([\S]+)\[([\d]*)\]\[([\d]*)\]", data_type)
    assert res is not None
    return res.group(1), res.group(2), res.group(3)


def is_set_type(data_type):
    """Check if the data type is a set type."""
    return re.search(r"Set\<([\S]+)\>", data_type) is not None


def get_set_type(data_type):
    """Get the set type."""
    res = re.search(r"Set\<([\S]+)\>", data_type)
    assert res is not None
    return res


def is_map_type(data_type):
    """Check if the data type is a map type."""
    if not isinstance(data_type, (str, bytes)):
        return False  # 或者抛出异常，取决于您的需求
    return re.search(r"map\<([\S]+), ([\S]+)\>", data_type) is not None


def get_map_type(data_type):
    """Get the map type and value types."""
    res = re.search(r"map\<([\S]+), ([\S]+)\>", data_type)
    assert res is not None
    return res.group(1), res.group(2)


def is_cpp_array(data_type):
    """Check if the data type is a C++ array type."""
    return re.search(r"std::array<([^,]+),\s*(\d+)>", data_type) is not None


def get_cpp_array_type(data_type):
    """Get the C++ array type and size."""
    res = re.search(r"std::array<([^,]+),\s*(\d+)>", data_type)
    assert res is not None
    return res.group(1), res.group(2)

def is_cpp_vector(data_type):
    """Check if the data type is a C++ vector type."""
    return re.search(r"std::vector<(\w+)_t>", data_type) is not None


def get_cpp_vector_type(data_type):
    """Get the C++ vector type."""
    res = re.search(r"std::vector<(\w+)_t>", data_type)
    assert res is not None
    return res.group(1)


def lower_first(value):
    """Lower the first letter of a string."""
    return value[0].lower() + value[1:]


def upper_first(value):
    """Upper the first letter of a string."""
    return value[0].upper() + value[1:]


def hex_to_int(value):
    """Convert a hex string to an integer."""
    return int(value, 16)

def get_byte_type(byte_num):
    """Obtain the type of byte count"""
    return (byte_num * 8)

def ns_colon_convert(namespace):
    """Convert a namespace list to a string with '::' separator."""
    ns_str = '::'.join(namespace)
    return ns_str

def ns_prime_convert(namespace):
    """Convert a namespace list to a string with '/' separator."""
    ns_str = '/'.join(namespace)
    return ns_str

def ns_segemtation_convert(delimiter, namespace):
    """Convert a namespace list to a string with a specified delimiter."""
    if not namespace:
        return ''
    ns_str = delimiter.join(namespace)
    ns_str = ns_str + delimiter
    return ns_str

def ns_segemtation_convert_prefix_dds(delimiter, namespace):
    """Convert a namespace list to a string with a specified delimiter."""
    if not namespace:
        return ''
    ns_str = "fastdds_types" + delimiter
    ns_str += delimiter.join(namespace)
    ns_str = ns_str + delimiter
    return ns_str

def ns_prefix_convert(namespace):
    """Convert a namespace list to a string."""
    # 将数组中的字符串首字母大写拼成一个字符串
    capitalized_string = ''.join(word.capitalize() for word in namespace)

    # 将拼成的字符串的首字母小写
    ns_str = capitalized_string[0].lower() + capitalized_string[1:]
    return ns_str

def gen_hash(hash_str):
    return hashlib.sha256(hash_str.encode()).hexdigest()[:4]

def com_type2dds_type(type):
    std_types_map = {
        "bool": "boolean",
        "float": "float",
        "double": "double",
        "int8_t": "int8",
        "int16_t": "int16",
        "int32_t": "int32",
        "int64_t": "int64",
        "uint8_t": "uint8",
        "uint16_t": "uint16",
        "uint32_t": "uint32",
        "uint64_t": "uint64",
        "std::string": "string",
    }
    return std_types_map[type]
