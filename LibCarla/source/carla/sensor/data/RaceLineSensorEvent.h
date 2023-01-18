// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/rpc/ActorId.h"
#include "carla/geom/Vector3D.h"
#include "carla/sensor/data/Array.h"

namespace carla {
namespace sensor {
namespace data {

  class RaceLineSensorEvent : public Array<geom::Vector3D> {
  public:

    explicit RaceLineSensorEvent(RawData &&data)
      : Array<geom::Vector3D>(0u, std::move(data)) {}
  };

} // namespace data
} // namespace sensor
} // namespace carla
