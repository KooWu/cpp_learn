# 使用流程

## 证书与密钥
先用gen_mqtt_key_cert.sh生成密钥和身份证书
需要根据自己设置openssl的路径，保证通信双方的openssl的版本一致

## 通信验证
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../lib:../../lib64
./mosquitto -c broker.config
./mqtt_test 1(多个参数，打开安全功能)
python3 mqtt_test.py