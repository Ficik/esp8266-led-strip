# ESP8266 MQTT Led Strip

- Adressable led strip (ws2811) connected to pin D4 on nodemcu board
- subscribes to mqtt messages sent from openhab to MQTT_SUBSCRIBE topic
  - state (payload ON, OFF)
  - brightness (payload 0-100)
  - color (hsv value: 0-360,0-100,0-100)
- reports back state to mqtt
  - MQTT_SUBSCRIBE/state (ON, OFF)
  - MQTT_SUBSCRIBE/color (hsv value)
  - MQTT_SUBSCRIBE/brightness (0-100)

Openhab item config:

        Color    Workbench_Color       "LED Strip"        <light>         (Workbench, gLight)  ["Lighting"]     {mqtt=">[broker:/workbench/light:command:*:default],<[broker:/workbench/light/color:state:default]"}
        Dimmer   Workbench_Brightness  "LED Strip"        <light>         (Workbench, gLight)  ["Lighting"]     {mqtt=">[broker:/workbench/light:command:*:default],<[broker:/workbench/light/brightness:state:default"}
        Switch   Workbench_Switch      "LED Strip"        <light>         (Workbench, gLight)  ["Lighting"]     {mqtt=">[broker:/workbench/light:command:*:default],<[broker:/workbench/light/state:state:default"}

## Setup

- copy and fill src/config.h.dist -> src/config.h
- pio run