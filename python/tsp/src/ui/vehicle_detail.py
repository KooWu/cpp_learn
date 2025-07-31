from PySide6.QtWidgets import *
from PySide6.QtCore import *
from PySide6.QtGui import *

class VehicleDetailPanel(QWidget):
    def __init__(self, vehicle_manager):
        super().__init__()
        self.vehicle_manager = vehicle_manager
        self.current_vehicle_id = None
        self.setup_ui()
        self.setup_connections()
        self.cntl_id = 0
        
    def setup_ui(self):
        layout = QVBoxLayout(self)
        
        # 创建选项卡
        self.tab_widget = QTabWidget()
        layout.addWidget(self.tab_widget)
        
        # 添加各个选项卡
        self.info_tab = self.create_info_tab()
        self.data_tab = self.create_data_tab()
        self.alert_tab = self.create_alert_tab()
        self.control_tab = self.create_control_tab()
        # self.chart_tab = self.create_chart_tab()
        
        self.tab_widget.addTab(self.info_tab, "basic info")
        self.tab_widget.addTab(self.data_tab, "real-time data")
        self.tab_widget.addTab(self.alert_tab, "alert data")
        self.tab_widget.addTab(self.control_tab, "control panel")
        # self.tab_widget.addTab(self.chart_tab, "chart")
        
    def create_info_tab(self):
        widget = QWidget()
        layout = QFormLayout(widget)
        
        self.id_label = QLabel()
        self.status_label = QLabel()
        self.update_time_label = QLabel()
        
        layout.addRow("vehicleID:", self.id_label)
        layout.addRow("status:", self.status_label)
        layout.addRow("update time:", self.update_time_label)
        
        return widget
        
    def create_data_tab(self):
        widget = QWidget()
        layout = QVBoxLayout(widget)
        
        self.data_table = QTableWidget()
        self.data_table.setColumnCount(3)
        self.data_table.setHorizontalHeaderLabels(["param", "value", "unit"])
        layout.addWidget(self.data_table)
        return widget

    def create_alert_tab(self):
        widget = QWidget()
        layout = QVBoxLayout(widget)
        
        self.alert_table = QTableWidget()
        self.alert_table.setColumnCount(2)
        self.alert_table.setHorizontalHeaderLabels(["item", "status"])
        layout.addWidget(self.alert_table)
        
        return widget
        
    def create_control_tab(self):
        widget = QWidget()
        layout = QGridLayout(widget)
        
        # 创建状态显示标签
        self.air_label = QLabel("air status:")
        self.door_label = QLabel("door status:")
        self.air_status = QLabel("unkown")
        self.door_status = QLabel("unkown")

        # 将状态显示标签添加到布局中
        layout.addWidget(self.air_label, 0, 0)  # 添加标签的描述
        layout.addWidget(self.air_status, 0, 1)         # 添加状态显示标签
        layout.addWidget(self.door_label, 1, 0) # 添加标签的描述
        layout.addWidget(self.door_status, 1, 1)        # 添加状态显示标签

        # 创建按钮
        self.off_btn = QPushButton("airconditioning off")
        self.on_btn = QPushButton("airconditioning on")
        self.lock_btn = QPushButton("door lock")
        self.unlock_btn = QPushButton("door unlock")

        # 设置按钮的样式
        self.off_btn.setStyleSheet("QPushButton:hover { background-color: #f44336; }")  # 红色
        self.on_btn.setStyleSheet("QPushButton:hover { background-color: #f44336; }")   # 红色
        self.lock_btn.setStyleSheet("QPushButton:hover { background-color: #f44336; }") # 红色
        self.unlock_btn.setStyleSheet("QPushButton:hover { background-color: #f44336; }") # 红色
        
        # 将按钮添加到布局中
        layout.addWidget(self.off_btn, 2, 0)
        layout.addWidget(self.on_btn, 2, 1)
        layout.addWidget(self.lock_btn, 3, 0)
        layout.addWidget(self.unlock_btn, 3, 1)

        # layout.setRowStretch(0, 1)  # 列0的拉伸因子为1
        # layout.setRowStretch(1, 1)  # 列1的拉伸因子为3，这样按钮会占据更多的空间
        # layout.setRowStretch(2, 3)  # 列0的拉伸因子为1
        # layout.setRowStretch(3, 3)  # 列1的拉伸因子为3，这样按钮会占据更多的空间

        return widget
        
    # def create_chart_tab(self):
    #     widget = QWidget()
    #     layout = QVBoxLayout(widget)
        
    #     self.speed_chart = RealTimeChart("speed")
    #     self.temp_chart = RealTimeChart("temperature")
        
    #     layout.addWidget(self.speed_chart)
    #     layout.addWidget(self.temp_chart)

        return widget

    def setup_connections(self):
        self.off_btn.clicked.connect(self.off_air)
        self.on_btn.clicked.connect(self.on_air)
        self.lock_btn.clicked.connect(self.lock_door)
        self.unlock_btn.clicked.connect(self.unlock_door)
        
        self.vehicle_manager.vehicle_updated.connect(self.on_vehicle_updated)
        
    def show_vehicle(self, vehicle_id):
        self.current_vehicle_id = vehicle_id
        self.update_display()
        
    def update_display(self):
        if not self.current_vehicle_id:
            return
            
        vehicle = self.vehicle_manager.get_vehicle(self.current_vehicle_id)
        if not vehicle:
            return
            
        # 更新基本信息
        self.id_label.setText(vehicle.id)
        self.status_label.setText(vehicle.status)
        self.update_time_label.setText(
            vehicle.last_update.strftime("%Y-%m-%d %H:%M:%S")
        )
        
        # 更新数据表格
        self.update_data_table(vehicle)
        self.update_alert_table(vehicle)

        self.air_status.setText(vehicle.air_status)
        self.door_status.setText(vehicle.door_status)
        
        # 更新图表
        # self.speed_chart.add_data(vehicle.speed)
        # self.temp_chart.add_data(vehicle.temperature)
        
    def update_data_table(self, vehicle):
        self.data_table.setRowCount(0)
        data = [
            ("speed", vehicle.speed, "km/h"),
            ("temperature", vehicle.temperature, "°C"),
            ("battery", vehicle.battery, "%"),
            ("longitude", vehicle.longitude, ""),
            ("latitude", vehicle.latitude, "")
        ]

        for row, (param, value, unit) in enumerate(data):
            self.data_table.insertRow(row)
            self.data_table.setItem(row, 0, QTableWidgetItem(param))
            self.data_table.setItem(row, 1, QTableWidgetItem(str(value)))
            self.data_table.setItem(row, 2, QTableWidgetItem(unit))

    def update_alert_table(self, vehicle):
        self.alert_table.setRowCount(0)
        data = [
            ("lights", vehicle.lights),
            ("airbag", vehicle.airbag),
            ("air-conditioning", vehicle.air_conditioning),
            ("window", vehicle.window),
            ("door", vehicle.door)
        ]
        
        for row, (param, value) in enumerate(data):
            self.alert_table.insertRow(row)
            self.alert_table.setItem(row, 0, QTableWidgetItem(param))
            self.alert_table.setItem(row, 1, QTableWidgetItem(value))
            
    def on_vehicle_updated(self, vehicle_id):
        if vehicle_id == self.current_vehicle_id:
            self.update_display()
            
    def off_air(self):
        if self.current_vehicle_id:
            self.cntl_id = self.cntl_id + 1
            msg_json = {"type": "cntl", "cntl_id": self.cntl_id, "item": "air-conditioning", "cmd": "off"}
            self.vehicle_manager.publish_command(self.current_vehicle_id, "cntl", msg_json)
            
    def on_air(self):
        if self.current_vehicle_id:
            self.cntl_id = self.cntl_id + 1
            msg_json = {"type": "cntl", "cntl_id": self.cntl_id, "item": "air-conditioning", "cmd": "on"}
            self.vehicle_manager.publish_command(self.current_vehicle_id, "cntl", msg_json)
            
    def lock_door(self):
        if self.current_vehicle_id:
            self.cntl_id = self.cntl_id + 1
            msg_json = {"type": "cntl", "cntl_id": self.cntl_id, "item": "door", "cmd": "lock"}
            self.vehicle_manager.publish_command(self.current_vehicle_id, "cntl", msg_json)
            
    def unlock_door(self):
        if self.current_vehicle_id:
            self.cntl_id = self.cntl_id + 1
            msg_json = {"type": "cntl", "cntl_id": self.cntl_id, "item": "door", "cmd": "unlock"}
            self.vehicle_manager.publish_command(self.current_vehicle_id, "cntl", msg_json)