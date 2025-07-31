import paho.mqtt.client as mqtt
import json
import ssl
from utils.logger import get_logger

class MQTTClient:
    def __init__(self, config, vehicle_manager):
        self.config = config
        self.vehicle_manager = vehicle_manager
        self.logger = get_logger()
        self.is_connected = False

        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect

        if not config.is_anonymous:
            self.logger.info(f"MQTT用户名: {config.username}")
            self.logger.info(f"MQTT密码: {config.password}")
            if config.username and config.password:
                self.client.username_pw_set(
                    config.username, 
                    config.password
                )

        # 设置 TLS
        if config.need_auth:
            self.logger.info(f"MQTT需要TLS认证")
            self.client.tls_set(ca_certs=config.ca_cert, certfile=config.client_cert, keyfile=config.client_key,
                        tls_version=ssl.PROTOCOL_TLSv1_2)
            
    def start(self):
        try:
            self.client.connect(self.config.broker, self.config.port, 60)
            self.client.loop_start()
        except Exception as e:
            self.logger.error(f"MQTT连接失败: {e}")
            
    def stop(self):
        self.client.loop_stop()
        self.client.disconnect()
        
    def on_connect(self, client, userdata, flags, rc):
        self.logger.info(f"MQTT已连接: {rc}")
        self.is_connected = True
        for topic in self.config.topics:
            self.logger.info(f"subscribe topic: {topic}")
            client.subscribe(topic)

    def login_subscribe(self, vin, client, is_subscribe):
        print(f"login_subscribe: {vin}, {is_subscribe}")
        topic = "vhl"
        topic += f"/{vin}/"
        topic += "cntl_reply"
        if is_subscribe:
            client.subscribe(topic)
        else:
            client.unsubscribe(topic)

    def on_message(self, client, userdata, msg):
        try:
            data = json.loads(msg.payload)
            print(f"recv: {msg.topic}, {data}")
            if data["type"] == "login":
                is_success = self.vehicle_manager.handle_login_msg(data)
                if is_success:
                    msg = "login success"
                else:
                    msg = "login failed"
                msg_json = {"type": "login_reply", "success": is_success, "msg": msg}
                payload = json.dumps(msg_json)
                topic = "tsp"
                topic += f"/{data['vin']}/"
                topic += "reply"
                client.publish(topic, payload)
                self.login_subscribe(data["vin"], client, True)
            elif data["type"] == "logout":
                self.login_subscribe(data["vin"], client, False)
                self.vehicle_manager.handle_logout_msg(data)
            elif data["type"] == "data":
                self.vehicle_manager.handle_data_msg(data)
            elif data["type"] == "alert":
                self.vehicle_manager.handle_alert_msg(data)
            elif data["type"] == "cntl":
                self.vehicle_manager.handle_cntl_msg(data)
        except json.JSONDecodeError:
            self.logger.error(f"无效的JSON数据: {msg.payload}")
    
    def publish(self, topic, payload):
        if self.is_connected:
            print(f"send: {topic}, {payload}")
            self.client.publish(topic, payload)
        else:
            self.logger.warning(f"MQTT未连接，无法发布消息: {topic}, {payload}")
            
    def on_disconnect(self, client, userdata, rc):
        self.logger.warning(f"MQTT已断开: {rc}")
        self.is_connected = False