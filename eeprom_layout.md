# EEPROM Layout

This document has all the Information about the eeprom layout.

## Main Layout

| Start-Byte | End-Byte | Content                      |
|:---------- |:--------:| ----------------------------:|
| 0          | 0        | valid configuration bitfield |
| 1          | 1        | valid gadget bitfield        |
| 2          | 2        | valid remote bitfield        |
| 3          | 3        | ir_recv pin                  |
| 4          | 4        | ir_send pin                  |
| 5          | 25       | id                           |
| 26         | 51       | wifi_ssid                    |
| 52         | 102      | wifi_pw                      |
| 103        | 118      | mqtt_ip                      |
| 119        | 125      | mqtt_port                    |
| 126        | 176      | mqtt_user                    |
| 176        | 226      | mqtt_pw                      |

## Valid Configuration Bitfield

Stores whether an eeprom section contains valid configuration info

| Bit | Content   |
|:--- |:---------:|
| 0   | ID        |
| 1   | WIFI_SSID |
| 2   | WIFI_PW   |
| 3   | MQTT_IP   |
| 4   | MQTT_PORT |
| 5   | MQTT_USER |
| 6   | MQTT_PW   |
| 7   |           |

## Valid Gadget Bitfield

Stores whether an eeprom section contains valid gadget configuration info

| Bit | Content |
|:--- |:-------:|
| 0   |         |
| 1   |         |
| 2   |         |
| 3   |         |
| 4   |         |
| 5   |         |
| 6   |         |
| 7   |         |

## Valid Remote Bitfield

Stores whether an eeprom section contains valid remote configuration info

| Bit | Content |
|:--- |:-------:|
| 0   |         |
| 1   |         |
| 2   |         |
| 3   |         |
| 4   |         |
| 5   |         |
| 6   |         |
| 7   |         |