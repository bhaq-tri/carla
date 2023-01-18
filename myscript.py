from pprint import pprint as print

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

raceline = world.try_spawn_actor(sensor_blueprints[0], spawn_points[spawn_idx], vehicle)
# print(f"Type of 'raceline' {type(raceline)}")
# spawn_idx+=1

# print(dir(raceline))
def callback(vectors):
    print("Received new data:")
    for vector3d in vectors:
        print(f"   ({vector3d.x}, {vector3d.y}, {vector3d.z})")
raceline.listen(callback)


new_vec = [carla.Vector3D(i*1.0, i*2.0, i*3.0) for i in range(2,5)]
raceline.update_spline(new_vec)
