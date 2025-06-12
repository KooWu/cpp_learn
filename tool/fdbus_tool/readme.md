# 仓库说明
    fdbus通信框架生成器，减少开发者封装业务接口工作，开发者只需要专注业务开发，定义通信矩阵表excel

# 原理
    参考kb链接：https://kb.cvte.com/pages/viewpage.action?pageId=463464325&src=contextnavpagetreemode

# 使用

```shell
./excel2x.py fdbus_gen -h
usage: excel2x.py fdbus_gen [-h] [-x XLSX] [-o OUTPUT] [--debug DEBUG] [--sample SAMPLE] [--target_app TARGET_APP]

options:
  -h, --help            show this help message and exit #帮助说明
  -x XLSX, --xlsx XLSX  execl that need to be converted (default: None) # 输入的excel文件，必选参数
  -o OUTPUT, --output OUTPUT #输出路径，默认当前目录下的output目录
                        output file dir (default: output)
  --debug DEBUG         debug mode (default: True) # 是否开启调试模式，调试模式会输出中间的json
  --sample SAMPLE       Generate sample project or not # 是否输出示例工程文件
  --target_app TARGET_APP # 输出目标应用，默认全部输出
                        target app (default: all)
./excel2x.py fdbus_gen -x samples/tbox_app.xlsx  -o output
```

# 示例

```shell
#初次开发
./excel2x.py fdbus_gen -x samples/fdbus_desc.xlsx  -o output --sample true
#迭代开发，替换原工程中的fdbus_gen和proto_gen
./excel2x.py fdbus_gen -x samples/fdbus_desc.xlsx  -o output
```
