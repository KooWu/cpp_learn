## gperftools使用说明
google-pprof分析工具的安装目录
sudo apt-get install google-perftools libgoogle-perftools-dev

> cpu
./gperftools_test --cpu-profiler
google-pprof --text ./gperftools_test my_program.prof

> 内存分析
开启监控
HEAPPROFILE=/tmp/ HEAPCHECK=normal ./gperf
单次快照分析
google-pprof --show_bytes --lines --text ./gperf ".0001.heap"
快照对比分析
google-pprof --show_bytes --lines --text ./gperf --base="before_leak.0002.heap" "before_leak.0003.heap"

google-pprof --lib_prefix="/opt/qcm6125/sysroots/aarch64-oe-linux/usr/lib,/home/yaoxingwei0382/mtk8678/tbox/demo_prj/install/qcm6125/lib,/opt/qcm6125/sysroots/aarch64-oe-linux/lib" --show_bytes --lines --text ./gperf --base="before_leak.0002.heap" "before_leak.0003.heap"