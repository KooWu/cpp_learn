from PySide6.QtCore import QObject, Signal
from dataclasses import dataclass
from datetime import datetime
import json
# from utils.config import Config

@dataclass
class VehicleData:
    id: str
    status: str
    last_update: datetime
    temperature: float = 50.0
    battery: float = 100.0
    speed: float = 0.0
    latitude: float = 0.0
    longitude: float = 0.0
    lights: str = "normal"
    airbag: str = "normal"
    air_conditioning: str = "normal"
    window: str = "normal"
    door: str = "normal"
    air_status: str = "off"
    door_status: str = "lock"

class VehicleManager(QObject):
    vehicle_updated = Signal(str)  # 车辆数据更新信号
    vehicle_status_changed = Signal(str, str)  # 车辆状态变化信号
    
    def __init__(self, vehicle_whitelist_config):
        super().__init__()
        self.mqtt_client = None
        self.vehicles = {}
        self.vehicle_whitelist_config = vehicle_whitelist_config
        if self.vehicle_whitelist_config.enabled:
            for vehicle in self.vehicle_whitelist_config.vehicles:
                self.vehicles[vehicle] = VehicleData(
                    id=vehicle,
                    status='offline',
                    last_update=datetime.now()
                )

    def handle_login_msg(self, data: dict):
        vehicle_id = data["vin"]
        if self.vehicle_whitelist_config.enabled:
            if vehicle_id in self.vehicle_whitelist_config.vehicles:
                self.update_vehicle(vehicle_id, data, "online")
            else:
                return False
        else:
            self.update_vehicle(vehicle_id, data, "online")
        return True

    def handle_logout_msg(self, data: dict):
        self.update_vehicle(data["vin"], data, "offline")

    def handle_data_msg(self, data: dict):
        if data["vin"] not in self.vehicles:
            return

        vehicle = self.vehicles[data["vin"]]
        vehicle.speed = data["speed"]
        vehicle.battery = data["battery"]
        vehicle.temperature = data["temperature"]
        vehicle.longitude = data["location"]["longitude"]
        vehicle.latitude = data["location"]["latitude"]
        vehicle.last_update = datetime.now()
        
        # 发送信号
        self.vehicle_updated.emit(data["vin"])

    def handle_alert_msg(self, data: dict):
        if data["vin"] not in self.vehicles:
            return

        vehicle = self.vehicles[data["vin"]]
        vehicle.lights = data["lights"]
        vehicle.airbag = data["airbag"]
        vehicle.air_conditioning = data["air_conditioning"]
        vehicle.window = data["window"]
        vehicle.door = data["door"]
        vehicle.last_update = datetime.now()
        
        # 发送信号
        self.vehicle_updated.emit(data["vin"])

    def handle_cntl_msg(self, data: dict):
        if data["vin"] not in self.vehicles:
            return

        vehicle = self.vehicles[data["vin"]]
        if data["item"] == "air-conditioning":
            vehicle.air_status = data["status"]
        elif data["item"] == "door":
            vehicle.door_status = data["status"]
        vehicle.last_update = datetime.now()
        
        # 发送信号
        self.vehicle_updated.emit(data["vin"])

    def update_vehicle(self, vehicle_id: str, data: dict, status: str):
        if vehicle_id not in self.vehicles:
            self.vehicles[vehicle_id] = VehicleData(
                id=vehicle_id,
                status='offline',
                last_update=datetime.now()
            )
            
        vehicle = self.vehicles[vehicle_id]
        old_status = vehicle.status
        vehicle.status = status
        vehicle.last_update = datetime.now()
        
        # 发送信号
        self.vehicle_updated.emit(vehicle_id)
        if old_status != vehicle.status:
            self.vehicle_status_changed.emit(vehicle_id, vehicle.status)
        return True

    def get_vehicle(self, vehicle_id: str) -> VehicleData:
        return self.vehicles.get(vehicle_id)
        
    def get_all_vehicles(self) -> list:
        return list(self.vehicles.values())

    def publish_command(self, vehicle_id: str, command: str, data: dict):
        if self.mqtt_client is not None:
            payload = json.dumps(data)
            self.mqtt_client.publish(f"tsp/{vehicle_id}/{command}", payload)