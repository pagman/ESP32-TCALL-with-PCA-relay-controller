# ESP32-TCALL-with-PCA-relay-controller-MQTT
testing
install ssh and web terminal
mqtt client
from terminal install mosquitto apk add --upgrade mosquitto


switch:
  - platform: mqtt # Again, it's an MQTT device
    name: "LED Switch 1" # Choose an easy-to-recognize name
    state_topic: "a" # Topic to read the current state
    command_topic: "a" # Topic to publish commands
    payload_on: "on0" # or "on", depending on your MQTT device
    payload_off: "off0" # or "off", depending on your MQTT device


#mqtt broker
active: false
folder: mosquitto
require_certificate: false
anonymous: true
