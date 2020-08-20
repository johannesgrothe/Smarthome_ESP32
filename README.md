# Smarthome_ESP32
## The Project
This project implements a smarthome system to be used with the ESP32. You can run a wide array of different gadgets (lamps, doorbell, temperature sensors, light switches) being controlled by electrical signals or infrared signals, and control them using infrared, network or even other local gadgets (for example use a light switch to control a light running on the same chip without any network needed).

The goal is to create a great, customizable smarthome environment with a great bit of redundancy. Being able to control the same lightbulb by using an ir remote, siri or a lightswitch seems like a great alternative to just not having light when the router is down for some reason.

## Documentation
We try to create great documentation while developing to make everyone able to either build their own gadgets or even contribute to this project.
This readme will be updated as often as possible to contain starting instruction, there is a UML diagram in the "documentation" folder that can be viewed with draw.io web or desktop.

## Initialization:

#### CLion
##### Best solution:
1. First, install platformio core on your operating system ([see here](https://docs.platformio.org/en/latest/core/installation.html#installation-methods)).
2. Download and install the PlatformIO plugin, right-click on the platformio.ini (which should have an platformio-icon now) and click re-init.
3. Open `CMakeLists.txt` and there should be a blue Banner at the top (in CLion) to Load the CMake Project, click on it.
4. Goto CLion -> preferences -> Build, Execution, Deployment -> CMake and create a new Profile (refer to the More Help section). 
   The profile should automatically have the name esp32cam.
   Delete all the other profiles.
5. To run, go to the build profile selection at the top, click "Edit Configurations..." and add any PlatformIO-Configuration you like.
    Delete all the CMake build profiles.

##### Alternative Solution:
1. First, install platformio core on your operating system ([see here](https://docs.platformio.org/en/latest/core/installation.html#installation-methods)).
2. Open project root in terminal (or use CLions own terminal tab which is on project root by default) and run `platformio init --ide clion`. To build use `platformio run` or `platformio run --target upload` to upload.


##### More Help:
In case you encounter a problem while building the project, please refer to this site: [docs.platformio](https://docs.platformio.org/en/latest/integration/ide/clion.html#clion-does-not-load-project-build-environments-from-platformio-ini).


#### VS Code
Download and install the PlatformIO IDE plugin. To build the project open the PlatformIO Tab at the side of the window and click 'run'.

## Configuration
### General
The device accepts configurations as json structures.
#### Basic Structure
The json has the following structure at root:
```
{
  "id" : "???",
  "gadgets": [ ],
  "network": { },
  "connectors": { },
  "connector-mapping": { },
  "gadget-remote": { },
  "code-remote": { },
  "event-remote": { }
}
```

#### Network
```
  "network": {
    "type": "mqtt",
    "config": {
      "wifi_ssid": "testwlan",
      "wifi_password": "testpw",
      "ip": "192.168.178.xxx",
      "port": "1883",
      "mqtt_username": null,
      "mqtt_password": null
    }
  },
```

### Connectors
#### IR
```
"ir": {
  "recv_pin": xx,
  "send_pin": xx
}
```
   
#### MQTT_G
```
"mqtt": {
  "ip": "xxx.xxx.xxx.xxx",
  "port": "1883"
}
```

### Gadgets
#### sh_lamp_basic
```
{
 "type": "sh_lamp_basic",
 "name": "Testlampe 2",
 "lamp_type_": 0,
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
  "lamp_type_": 0,
  "pin": xx,
  "length": "xx",
  "mapping": {
    "toggleStatus": [1, 16],
    "turnOn": [2, 17],
    "turnOff": [3, 18]
  }
}
```

## Custom Gadgets
The whole system is designed to make it easy to create custom gadgets.
All Gadgets are split between logic and hardware implementation. For example, the `SH_Lamp` implements all the communication with the remotes and refreshing the local status. The hardware implementation `SH_Lamp_Basic` only needs to implement the constructor and the `refresh()`-method, as would any other hardware implementation that does not need any crazy custom features.
All you need to do to create a custom gadget is create a class inheriting from SH_Gadget (or any subclass if you want to just create a custom hardware implementation for a already created logic class) and code everything you want to use.

Then, add a statement of the pattern below to the `gadget_library.h`:
```
// my_class.h
else if (strcmp(type, "my_class") == 0) {
  new_gadget = new MY_Class(gadget_json);
}
```

And you're done.