from PySide6.QtWidgets import *
from PySide6.QtCore import *
from .vehicle_list import VehicleListPanel
from .vehicle_detail import VehicleDetailPanel

class VehicleMonitorUI(QMainWindow):
    def __init__(self, vehicle_manager):
        super().__init__()
        self.vehicle_manager = vehicle_manager
        self.setup_ui()
        self.setup_connections()
        
    def setup_ui(self):
        self.setWindowTitle("vehicle monitor")
        self.resize(1200, 800)
        
        # 创建中心widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        # 创建主布局
        layout = QHBoxLayout(central_widget)
        
        # 创建分割器
        splitter = QSplitter(Qt.Horizontal)
        layout.addWidget(splitter)
        
        # 创建左右面板
        self.list_panel = VehicleListPanel(self.vehicle_manager)
        self.detail_panel = VehicleDetailPanel(self.vehicle_manager)
        
        splitter.addWidget(self.list_panel)
        splitter.addWidget(self.detail_panel)
        
        # 设置分割比例
        splitter.setStretchFactor(0, 1)
        splitter.setStretchFactor(1, 2)
        
    def setup_connections(self):
        # 连接信号
        self.list_panel.vehicle_selected.connect(
            self.detail_panel.show_vehicle
        )