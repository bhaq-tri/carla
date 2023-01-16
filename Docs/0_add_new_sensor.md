# How to add a new Actor

[Reference:](tuto_D_create_sensor.md)

## Notes:

1. Only blueprint actors can be **easily** added to the PythonApi. To spawn a new actor is:

```python
# Find all the blueprints
blueprints = world.get_blueprints_library().filter("*")
# Find spawn points
spawn_locations = world.get_map().get_spawn_locations()
# Add the actor
my_actor = world.try_spawn_actor(bp, spawn_location)
```

2. Throughput communication!

```text
                                   ┌────────────────────────────────────────────────────┐
                                   │                     LibCarla                       │
┌───────┐     ┌────────────────┐   │  ┌──────────┐         ┌──────────┐   ┌───────────┐ │   ┌──────────────┐
│  UE4  │◄────►  Carla Plugin  ◄───┼─►│  Server  │◄───────►│  Client  │◄─►│  C++ API  ◄─┼──►│  Python API  │
└───────┘     └────────────────┘   │  └──────────┘   TCP   └──────────┘   └───────────┘ │   └──────────────┘
                                   └────────────────────────────────────────────────────┘
└──────────────────────────────────────────────────┘      └──────────────────────────────────────────────────┘
               Simulator                                                            Client
```

- Carla Plugin directly communicates with UE4 with the Unreal protocol.
- The server is outside the scope of Unreal and needs the proper connections in place.

  - See [ActorFactory](../LibCarla/source/carla/client/detail/ActorFactory.cpp), it uses the description to construct the correct implementation of the `Client` actor when requested from the `Python API` using the `try_spawn_actor`.
  - Unclear how the `Server`<-->`Client` comm takes place over TCP.
  - The same class also exists with the same name at [CarlaActorFactory](../Unreal/CarlaUE4/Plugins/Carla/Source/Carla/Actor/CarlaActorFactory.h)

- The  [rpc](../LibCarla/source/carla/rpc/ActorState.h) actor state provides some guidance
- This is the blueprint actor [blueprint actor](../Unreal/CarlaUE4/Plugins/Carla/Source/Carla/Sensor/DataLineSensor.cpp)
- Figure out how the PythonApi Actor talks to the plugins blue print actor.
