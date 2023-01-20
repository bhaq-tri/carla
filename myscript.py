import time

import carla

# Connect
client = carla.Client('localhost', 2000)

# Helpers
world = client.get_world()
map_world = world.get_map()
spectator = world.get_spectator()
spawn_points = map_world.get_spawn_points()

blueprints = [bp for bp in world.get_blueprint_library().filter('*')]
blueprints_id = [each.id for each in blueprints].sort()

# Create a pedestrian
spawn_idx = 10
# walker_blueprints = [bp for bp in world.get_blueprint_library().filter('walker.*')]
# walker = world.try_spawn_actor(walker_blueprints[0], spawn_points[spawn_idx])
# spawn_idx+=1

# ai_blueprints = [bp for bp in world.get_blueprint_library().filter('*.ai.*')]
# ai = world.try_spawn_actor(ai_blueprints[0], spawn_points[spawn_idx], walker)
# spawn_idx+=1

# Create a vehicle
transform = world.get_map().get_spawn_points()[1]
vehicle_blueprints = world.get_blueprint_library().filter('*vehicle*')
for blueprint in vehicle_blueprints:
    print(blueprint.id)
vehicle = world.try_spawn_actor(vehicle_blueprints[2], transform)
spawn_idx+=1

# Get all the other blueprints
sensor_blueprints = [bp for bp in world.get_blueprint_library().filter('*race_line*')]
for blueprint in sensor_blueprints:
    print(blueprint.id)

# Spawn the actor
# dataline = world.try_spawn_actor(sensor_blueprints[-1], spawn_points[spawn_idx])
# spawn_idx+=1
# print(f"Type of 'dataline' {type(dataline)}")
# print(dir(dataline))

raceline = world.try_spawn_actor(sensor_blueprints[0], spawn_points[spawn_idx])
# print(f"Type of 'raceline' {type(raceline)}")
# spawn_idx+=1

# print(dir(raceline))
def callback(vectors):
    print("Received new data:")
    for vector3d in vectors:
        print(f"   ({vector3d.x}, {vector3d.y}, {vector3d.z})")
raceline.listen(callback)


new_vec = [carla.Vector3D(i*200.00, i*200.0, 50) for i in range(5)]
color_vec = [0.0, 0.5, 1.0, 0.5, 0.0]
# raceline.update_spline(new_vec)
time.sleep(2.0)
spectator.set_transform(raceline.get_transform())


other =[
    carla.Vector3D(1000, -400, 0),
    carla.Vector3D(4000, -200, 0),
    carla.Vector3D(3000,  400, 0),
    carla.Vector3D(1500,  600, 0),
    carla.Vector3D(3000,  800, 0)
]
