## Light (without state publish - not yet at least)

```yaml
light:
  - platform: mqtt
    name: "Hallway"
    # state_topic: "office/rgb1/light/status"
    command_topic: "home/cards/0/relays/0/set"
    qos: 0
    payload_on: "ON"
    payload_off: "OFF"
    optimistic: true
```

## Automation

```yaml
automation:
- alias: Toggle hallway light on short press
  trigger:
    platform: mqtt
    topic: home/cards/0/inputs/0
    payload: 'SHORT_PRESS'
  action:
    service: light.toggle
    entity_id: light.hallway
- alias: Toggle bedroom light on short press
  trigger:
    platform: mqtt
    topic: home/cards/0/inputs/1
    payload: 'SHORT_PRESS'
  action:
    service: light.toggle
    entity_id: light.bedroom
```
