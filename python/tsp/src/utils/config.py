import yaml
from dataclasses import dataclass

@dataclass
class MQTTConfig:
    broker: str
    port: int
    is_anonymous: bool
    username: str
    password: str
    need_auth: bool
    ca_cert: str
    client_cert: str
    client_key: str
    topics: list

@dataclass
class VehilceWhiteListConfig:
    enabled: bool
    vehicles: list

class Config:
    def __init__(self, config_file='config/config.yaml'):
        with open(config_file, 'r') as f:
            config_data = yaml.safe_load(f)
            
        self.mqtt_config = MQTTConfig(**config_data['mqtt'])
        self.vehicle_whitelist_config = VehilceWhiteListConfig(**config_data['vehicle_whitelist'])