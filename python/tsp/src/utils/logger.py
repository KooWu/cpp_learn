import logging
import sys
import os
from datetime import datetime

def setup_logger(name='vehicle_monitor', level=logging.INFO):
    # 创建logger
    logger = logging.getLogger(name)
    logger.setLevel(level)
    
    # 创建控制台处理器
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setLevel(level)

    # 获取当前文件所在的目录路径
    current_dir = os.getcwd()

    # 定义日志目录的路径为当前目录下的logs文件夹
    log_dir = os.path.join(current_dir, 'logs')

    # 确保日志目录存在，如果不存在则创建
    os.makedirs(log_dir, exist_ok=True)

    # 创建文件处理器
    file_handler = logging.FileHandler(
        f'logs/vehicle_monitor_{datetime.now().strftime("%Y%m%d")}.log'
    )
    file_handler.setLevel(level)
    
    # 创建格式器
    formatter = logging.Formatter(
        '%(asctime)s - %(name)s - %(levelname)s - %(message)s'
    )
    console_handler.setFormatter(formatter)
    file_handler.setFormatter(formatter)
    
    # 添加处理器
    logger.addHandler(console_handler)
    logger.addHandler(file_handler)
    
    return logger

def get_logger(name='vehicle_monitor'):
    return logging.getLogger(name)