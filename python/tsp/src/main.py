import sys
from PySide6.QtWidgets import QApplication
from ui.main_window import VehicleMonitorUI
from core.mqtt_client import MQTTClient
from core.vehicle_manager import VehicleManager
from utils.config import Config
from utils.logger import setup_logger

def main():
    # 初始化配置
    config = Config()
    
    # 设置日志
    logger = setup_logger()
    
    # 创建应用
    app = QApplication(sys.argv)
    
    # 加载样式表
    with open('resources/styles/main.qss', 'r') as f:
        app.setStyleSheet(f.read())
    
    # 创建车辆管理器
    vehicle_manager = VehicleManager(config.vehicle_whitelist_config)
    
    # 创建MQTT客户端
    mqtt_client = MQTTClient(config.mqtt_config, vehicle_manager)
    vehicle_manager.mqtt_client = mqtt_client
    # 创建主窗口
    window = VehicleMonitorUI(vehicle_manager)
    window.show()
    
    # 启动MQTT客户端
    mqtt_client.start()
    
    # 运行应用
    return app.exec()

if __name__ == '__main__':
    sys.exit(main())