
#!/bin/bash
sdk_path=/home/yaoxingwei0382/my/cpp_learn/install/x86_sysroot/release
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${sdk_path}/lib:${sdk_path}/lib64
openssl_bin=${sdk_path}/bin/openssl
# 生成根CA私钥和证书
${openssl_bin} genrsa -out root_private.pem 2048
${openssl_bin} req -new -x509 -days 3650 -key root_private.pem -out root_cert.pem

# 生成服务器私钥和证书请求
${openssl_bin} genrsa -out server_private.pem 2048
${openssl_bin} req -new -key server_private.pem -out server_req.pem

# 使用根CA签名服务器证书
${openssl_bin} x509 -req -in server_req.pem -days 365 -CA root_cert.pem -CAkey root_private.pem -CAcreateserial -out server_cert.pem