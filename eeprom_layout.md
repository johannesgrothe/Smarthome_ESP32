# EEPROM Layout

This document has all the Information about the eeprom layout.

## Main Layout

| Start-Byte | End-Byte           | Content                      |
|:---------- |:------------------:| ----------------------------:|
| 0          | 0                  | valid configuration bitfield |
| 1          | 1                  | gadget count                 |
| 2          | 2                  | system settings bitfield     |
| 3          | 3                  | ir_recv pin                  |
| 4          | 4                  | ir_send pin                  |
| 5          | 5                  | radio_recv pin               |
| 6          | 6                  | radio_send pin               |
| 7          | 7                  | network_mode                 |
| 8          | 8                  | gadget_remote                |
| 9          | 9                  | code_remote                  |
| 10         | 10                 | event_remote                 |
| 15         | 35                 | id                           |
| 36         | 61                 | wifi_ssid                    |
| 62         | 112                | wifi_pw                      |
| 113        | 117                | mqtt_ip                      |
| 118        | 120                | mqtt_port                    |
| 121        | 171                | mqtt_user                    |
| 172        | 222                | mqtt_pw                      |
|            |                    | gadget_a_start               |
|            |                    | gadget_b_start               |
|            |                    | gadget_c_start               |
|            |                    | gadget_d_start               |
|            |                    | gadget_e_start               |
|            |                    | gadget_f_start               |
|            |                    | gadget_g_start               |
|            |                    | gadget_h_start               |
|            |                    | gadget_h_end                 |
|            | end of eeprom size | gadget storage               |

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

## Gadget Config

### Gadget Config Block

Layout of a gadget config in eeprom

| Bit         | Content                           |
|:----------- |:---------------------------------:|
| 0           | Gadget Config Bitfield            |
| 1           | Gadget Type                       |
| 2 + 3       | Config Length                     |
| 4 - a       | Base Config (Json String)         |
| (a + 1) - b | Code Mapping Config (Json String) |

### Gadget Config Bitfield

Layout of the gadget config bitfield

| Bit | Content              |
|:--- |:--------------------:|
| 0   | Gadget Remote Active |
| 1   | Event Remote Active  |
| 2   |                      |
| 3   |                      |
| 4   |                      |
| 5   |                      |
| 6   |                      |
| 7   |                      |