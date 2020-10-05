# EEPROM Layout

This document has all the Information about the eeprom layout.

## Main Layout

| Start-Byte | End-Byte | Content                      |
|:---------- |:--------:| ----------------------------:|
| 0          | 0        | valid configuration bitfield |
| 1          | 1        | valid gadget bitfield        |
| 2          | 2        | system settings bitfield     |
| 3          | 3        | ir_recv pin                  |
| 4          | 4        | ir_send pin                  |
| 5          | 5        | radio_pin                    |
| 6          | 6        | network_mode                 |
| 7          | 7        | gadget_remote                |
| 8          | 8        | code_remote                  |
| 9          | 9        | event_remote                 |
| 10         | 30       | id                           |
| 31         | 56       | wifi_ssid                    |
| 57         | 107      | wifi_pw                      |
| 108        | 123      | mqtt_ip                      |
| 124        | 130      | mqtt_port                    |
| 131        | 181      | mqtt_user                    |
| 182        | 232      | mqtt_pw                      |

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

## System Settings Bitfield

Stores information about the system status

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