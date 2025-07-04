#!/bin/bash

# 创建目录结构
mkdir -p cert
sdk_path=/home/yaoxingwei0382/my/cpp_learn/install/x86_sysroot/release
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${sdk_path}/lib:${sdk_path}/lib64
openssl_bin=${sdk_path}/bin/openssl

# 生成根 CA 私钥和自签名证书
if [ ! -f cert/mqtt_client2.crt ];then
${openssl_bin} req -x509 -newkey rsa:4096 -days 3650 -nodes \
    -keyout cert/rootMqttCA.key -out cert/rootMqttCA.crt \
    -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=MyRootCA"

# 生成服务器私钥和 CSR
${openssl_bin} req -newkey rsa:4096 -nodes -keyout cert/mqtt_server.key \
    -out cert/mqtt_server.csr \
    -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=localhost"

# 使用 SAN 扩展指定 IP 地址或域名（如果需要）
cat > cert/v3.ext <<-EOF
subjectAltName = IP:127.0.0.1,IP:10.10.24.37
EOF

# 使用根 CA 签名生成服务器证书
${openssl_bin} x509 -req -in cert/mqtt_server.csr \
    -CA cert/rootMqttCA.crt -CAkey cert/rootMqttCA.key -CAcreateserial \
    -out cert/mqtt_server.crt -days 365 -sha256 -extfile cert/v3.ext

# 生成客户端私钥和 CSR

${openssl_bin} req -newkey rsa:4096 -nodes -keyout cert/mqtt_client.key \
    -out cert/mqtt_client.csr \
    -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=mqtt_client.your_domain.com"

# 使用根 CA 签名生成客户端证书
${openssl_bin} x509 -req -in cert/mqtt_client.csr \
    -CA cert/rootMqttCA.crt -CAkey cert/rootMqttCA.key -CAcreateserial \
    -out cert/mqtt_client.crt -days 365 -sha256

# 生成客户端2私钥和 CSR

${openssl_bin} req -newkey rsa:4096 -nodes -keyout cert/mqtt_client2.key \
    -out cert/mqtt_client2.csr \
    -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=mqtt_client.your_domain2.com"

# 使用根 CA 签名生成客户端2证书
${openssl_bin} x509 -req -in cert/mqtt_client2.csr \
    -CA cert/rootMqttCA.crt -CAkey cert/rootMqttCA.key -CAcreateserial \
    -out cert/mqtt_client2.crt -days 365 -sha256
fi

# # 设置变量
# ROOT_CA_DIR="rootMqttCA"
# SERVER_DIR="mqtt_server"
# CLIENT_DIR="mqtt_client"
# SERVER_IP="127.0.0.1"  # 替换为实际的IP地址
# ROOT_CA_PASSWORD="your_root_ca_password"
# SERVER_PASSWORD="your_server_password"
# CLIENT_PASSWORD="your_client_password"

# # 创建目录结构
# mkdir -p ${ROOT_CA_DIR} ${SERVER_DIR} ${CLIENT_DIR}

# # 初始化序列号文件
# initialize_serial() {
#     local serial_file=$1
#     if [ ! -f "$serial_file" ]; then
#         echo "01" > "$serial_file"
#     fi
# }

# # 1. 创建根证书
# echo "创建根证书..."
# if [ ! -f ${ROOT_CA_DIR}/rootMqttCA.key ];then
#     ${openssl_bin} genrsa -aes256 -out ${ROOT_CA_DIR}/rootMqttCA.key 4096
# fi

# if [ ! -f ${ROOT_CA_DIR}/rootMqttCA.crt ];then
#     ${openssl_bin} req -x509 -new -nodes -key ${ROOT_CA_DIR}/rootMqttCA.key \
#         -keyform PEM \
#         -sha256 -days 3650 -out ${ROOT_CA_DIR}/rootMqttCA.crt \
#         -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=MyRootCA"
# fi

# # 初始化序列号文件
# initialize_serial "${ROOT_CA_DIR}/rootMqttCA.srl"

# # 2. 创建服务器证书
# echo "创建服务器证书..."
# if [ ! -f ${SERVER_DIR}/mqtt_server.key ];then
#     ${openssl_bin} genrsa -aes256 -out ${SERVER_DIR}/mqtt_server.key 4096
# fi

# if [ ! -f ${SERVER_DIR}/mqtt_server.csr ];then
#     ${openssl_bin} req -new -key ${SERVER_DIR}/mqtt_server.key \
#         -keyform PEM \
#         -out ${SERVER_DIR}/mqtt_server.csr \
#         -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=${SERVER_IP}"
# fi

# # 使用 SAN 扩展指定 IP 地址
# if [ ! -f ${SERVER_DIR}/mqtt_server.crt ];then
#     cat > ${SERVER_DIR}/v3.ext <<-EOF
# subjectAltName = IP:${SERVER_IP}
# EOF

#     ${openssl_bin} x509 -req -in ${SERVER_DIR}/mqtt_server.csr \
#         -CA ${ROOT_CA_DIR}/rootMqttCA.crt -CAkey ${ROOT_CA_DIR}/rootMqttCA.key \
#         -CAcreateserial -CAserial ${ROOT_CA_DIR}/rootMqttCA.srl \
#         -keyform PEM \
#         -extfile ${SERVER_DIR}/v3.ext \
#         -out ${SERVER_DIR}/mqtt_server.crt -days 365 -sha256
# fi

# # 初始化服务器序列号文件
# initialize_serial "${ROOT_CA_DIR}/mqtt_server.srl"

# # 3. 创建客户端证书
# echo "创建客户端证书..."
# if [ ! -f ${CLIENT_DIR}/mqtt_client.key ];then
#     ${openssl_bin} genrsa -aes256 -out ${CLIENT_DIR}/mqtt_client.key 4096
# fi

# if [ ! -f ${CLIENT_DIR}/mqtt_client.crt ];then
#     ${openssl_bin} req -new -key ${CLIENT_DIR}/mqtt_client.key \
#         -keyform PEM -passin \
#         -out ${CLIENT_DIR}/mqtt_client.csr \
#         -subj "/C=CN/ST=Zhejiang/L=Hangzhou/O=MyOrg/OU=MyUnit/CN=mqtt_client.your_domain.com"

#     # 初始化客户端序列号文件
#     initialize_serial "${ROOT_CA_DIR}/mqtt_client.srl"

#     ${openssl_bin} x509 -req -in ${CLIENT_DIR}/mqtt_client.csr \
#         -CA ${ROOT_CA_DIR}/rootMqttCA.crt -CAkey ${ROOT_CA_DIR}/rootMqttCA.key \
#         -CAcreateserial -CAserial ${ROOT_CA_DIR}/mqtt_client.srl \
#         -keyform PEM \
#         -out ${CLIENT_DIR}/mqtt_client.crt -days 365 -sha256
# fi