# Smarthome_ESP32

## Initialization:
#### Wifi-Credentials
Copy `wifi_credentials_DEFAULT.h`, change the two Definitions to fit your Network Credentials and rename it to `wifi_credentials.h`.

#### CLion
if you're using CLion IDE, open Project Root in Terminal (or use CLions own Terminal Tab) and run `platformio init --ide clion `


## Configuration
### General
#### Basic Structure
```
{
   "gadgets": [],
   "network": {},
   "connectors": {}
 }
```

#### Network
```
"network": {
        "type": "wifi",  
        "config": {  
          "ssid": "testwlan",  
          "password": "testpw"  
        }
}
```

### Connectors
#### IR
```
"ir": {
  "recv_pin": xx,
  "send_pin": xx
}
```
   
#### MQTT
```
"mqtt": {
  "ip": "xxx.xxx.xxx.xxx",
  "port": "1883"
}
```

#### Serial
```
"serial": {}
```

#### REST
```
"rest": {}
```
   
### Gadgets
#### sh_lamp_basic
```
{
 "type": "sh_lamp_basic",
 "name": "Testlampe 2",
 "lamp_type": 0,
 "pin": 2,
 "default_state": 1,
 "mapping": {
   "toggleStatus": [1, 16],
   "turnOn": [2, 17],
   "turnOff": [3, 18]
 }
}
```
   
#### sh_lamp_neopixel_basic
```
{
  "type": "sh_lamp_neopixel_basic",
  "name": "Testlampe NP",
  "lamp_type": 0,
  "pin": xx,
  "length": "xx",
  "mapping": {
    "toggleStatus": [1, 16],
    "turnOn": [2, 17],
    "turnOff": [3, 18]
  }
}
```

## Additional Gadgets
## Methods to be implemented:

### Code-Command
```
void applyMappingMethod(const char *method)
```

### Homebridge
```
bool getHomebridgeCharacteristics(char *buffer)
void applyHomebridgeCommand(const char *characteristic, int value)
```
