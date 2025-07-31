from PySide6.QtWidgets import *
from PySide6.QtCore import *
from PySide6.QtGui import *

class VehicleListPanel(QWidget):
    vehicle_selected = Signal(str)
    
    def __init__(self, vehicle_manager):
        super().__init__()
        self.vehicle_manager = vehicle_manager
        self.setup_ui()
        self.setup_connections()
        self.display_all_vehicles()
        
    def setup_ui(self):
        layout = QVBoxLayout(self)
        
        # 搜索框
        self.search_box = QLineEdit()
        self.search_box.setPlaceholderText("search vehicle...")
        layout.addWidget(self.search_box)
        
        # 过滤器
        filter_layout = QHBoxLayout()
        self.status_combo = QComboBox()
        self.status_combo.addItems(["all", "online", "offline"])
        filter_layout.addWidget(QLabel("status:"))
        filter_layout.addWidget(self.status_combo)
        layout.addLayout(filter_layout)
        
        # 车辆列表
        self.vehicle_list = QListWidget()
        layout.addWidget(self.vehicle_list)
        
        # 统计信息
        self.stats_group = QGroupBox("stats")
        stats_layout = QGridLayout()
        
        self.total_label = QLabel("total: 0")
        self.online_label = QLabel("online: 0")
        self.offline_label = QLabel("offline: 0")
        
        stats_layout.addWidget(self.total_label, 0, 0)
        stats_layout.addWidget(self.online_label, 0, 1)
        stats_layout.addWidget(self.offline_label, 1, 0)
        
        self.stats_group.setLayout(stats_layout)
        layout.addWidget(self.stats_group)
        
    def setup_connections(self):
        self.search_box.textChanged.connect(self.filter_vehicles)
        self.status_combo.currentTextChanged.connect(self.filter_vehicles)
        self.vehicle_list.itemClicked.connect(self.on_vehicle_selected)
        
        # 连接车辆管理器信号
        self.vehicle_manager.vehicle_updated.connect(self.update_vehicle)
        self.vehicle_manager.vehicle_status_changed.connect(
            self.update_vehicle_status
        )
        
    def update_vehicle(self, vehicle_id):
        vehicle = self.vehicle_manager.get_vehicle(vehicle_id)
        if not vehicle:
            return
            
        # 更新或添加列表项
        item = self.find_vehicle_item(vehicle_id)
        if not item:
            item = QListWidgetItem()
            item.setData(Qt.UserRole, vehicle_id)
            self.vehicle_list.addItem(item)
            
        # 更新显示
        item.setText(f"{vehicle_id} - {vehicle.status}")
        item.setIcon(self.get_status_icon(vehicle.status))
        
        # 更新统计信息
        self.update_statistics()
        
    def update_vehicle_status(self, vehicle_id, status):
        item = self.find_vehicle_item(vehicle_id)
        if item:
            item.setIcon(self.get_status_icon(status))
            
    def find_vehicle_item(self, vehicle_id):
        for i in range(self.vehicle_list.count()):
            item = self.vehicle_list.item(i)
            if item.data(Qt.UserRole) == vehicle_id:
                return item
        return None
        
    def get_status_icon(self, status):
        if status == 'online':
            return QIcon('resources/icons/online.png')
        elif status == 'offline':
            return QIcon('resources/icons/offline.png')
            
    def filter_vehicles(self):
        search_text = self.search_box.text().lower()
        status_filter = self.status_combo.currentText()
        
        for i in range(self.vehicle_list.count()):
            item = self.vehicle_list.item(i)
            vehicle_id = item.data(Qt.UserRole)
            vehicle = self.vehicle_manager.get_vehicle(vehicle_id)
            
            show = True
            # 搜索过滤
            if search_text and search_text not in vehicle_id.lower():
                show = False
            # 状态过滤
            if status_filter != "all" and vehicle.status != status_filter.lower():
                show = False
                
            item.setHidden(not show)
            
    def update_statistics(self):
        vehicles = self.vehicle_manager.get_all_vehicles()
        total = len(vehicles)
        online = sum(1 for v in vehicles if v.status == 'online')
        offline = sum(1 for v in vehicles if v.status == 'offline')
        
        self.total_label.setText(f"total: {total}")
        self.online_label.setText(f"online: {online}")
        self.offline_label.setText(f"offline: {offline}")
        
    def on_vehicle_selected(self, item):
        vehicle_id = item.data(Qt.UserRole)
        self.vehicle_selected.emit(vehicle_id)

    def display_all_vehicles(self):
        whitelist_vehicles = self.vehicle_manager.get_all_vehicles()  # 假设的方法
        self.vehicle_list.clear()  # 清空现有的车辆列表
        
        for vehicle in whitelist_vehicles:
            item = QListWidgetItem()
            item.setData(Qt.UserRole, vehicle.id)
            item.setText(f"{vehicle.id} - {vehicle.status}")
            item.setIcon(self.get_status_icon(vehicle.status))
            self.vehicle_list.addItem(item)
            
        # 更新统计信息
        self.update_statistics()