#include <mosquitto.h>
#include <iostream>
#include <cstring>

using namespace std;

void on_connect(struct mosquitto *mosq, void *userdata, int rc) {
    if(rc == 0) {
        cout << "Connected to broker with TLS." << endl;
        // 订阅主题
        mosquitto_subscribe(mosq, NULL, "test/python_topic", 0);
    } else {
        cout << "Failed to connect, return code " << rc << endl;
    }
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if(message->payloadlen) {
        cout << "Received message: " << string(static_cast<const char*>(message->payload), message->payloadlen)
             << " from topic " << message->topic << endl;
    } else {
        cout << "Received null message." << endl;
    }
}

void on_publish(struct mosquitto *mosq, void *userdata, int mid) {
    cout << "Message published." << endl;
}

void on_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    cout << "Subscribed to topic." << endl;
}

int main(int argc, char **argv) {
    struct mosquitto *mosq = NULL;

    // 初始化 mosquitto 库
    mosquitto_lib_init();

    // 创建 mosquitto 客户端实例
    mosq = mosquitto_new("cpp_client", true, NULL);
    if(!mosq) {
        cerr << "Error creating mosquitto instance" << endl;
        mosquitto_lib_cleanup();
        return 1;
    }

    // 设置回调函数
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_publish_callback_set(mosq, on_publish);
    mosquitto_subscribe_callback_set(mosq, on_subscribe);

    if (argc >= 2) {
        // 配置 TLS
        if(mosquitto_tls_set(mosq, 
        "/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/rootMqttCA.crt", 
        NULL, 
        "/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/mqtt_client2.crt", 
        "/home/yaoxingwei0382/my/cpp_learn/samples/mqtt/script/cert/mqtt_client2.key", 
        NULL)) {
            cerr << "Error setting up TLS" << endl;
            mosquitto_destroy(mosq);
            mosquitto_lib_cleanup();
            return 1;
        }

        // 设置 TLS 版本
        if(mosquitto_tls_opts_set(mosq, 1, "tlsv1.2", NULL)) {
            cerr << "Error setting TLS options" << endl;
            mosquitto_destroy(mosq);
            mosquitto_lib_cleanup();
            return 1;
        }
    }

    // mosquitto_username_pw_set(mosq, "your_username", "your_password");
    // 连接到 Broker
    int rc = mosquitto_connect(mosq, "localhost", 8883, 60);
    if(rc != MOSQ_ERR_SUCCESS) {
        cerr << "Error connecting to broker: " << mosquitto_strerror(rc) << endl;
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    // 发布一条消息
    const char* payload = "Hello from C++ client with TLS";
    mosquitto_publish(mosq, NULL, "test/cpp_topic", strlen(payload), payload, 0, false);

    // 循环处理网络事件和回调
    while(true) {
        mosquitto_loop(mosq, 1000, 1);
    }

    // 清理资源
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}