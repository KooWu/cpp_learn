# python_client.py
import paho.mqtt.client as mqtt
import ssl
import time

def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("test/cpp_topic")

def on_message(client, userdata, msg):
    print(f"Received message: {msg.payload.decode()} from topic {msg.topic}")


# 检查OpenSSL版本
print(f"OpenSSL版本: {ssl.OPENSSL_VERSION}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

broker_address = "localhost"
port = 8883

# 设置 TLS
client.tls_set(ca_certs="/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/rootMqttCA.crt",
               certfile="/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/mqtt_client.crt",
               keyfile="/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/mqtt_client.key",
               tls_version=ssl.PROTOCOL_TLSv1_2)

# 如果需要用户名和密码验证，取消注释以下两行
# client.username_pw_set(username="your_username", password="your_password")

client.connect(broker_address, port, 60)

client.loop_start()

# 发布一条消息
client.publish("test/python_topic", "Hello from Python client with TLS")

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Shutting down...")
finally:
    client.loop_stop()
    client.disconnect()