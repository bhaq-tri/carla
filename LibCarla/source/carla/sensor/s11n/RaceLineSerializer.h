// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/Memory.h"
#include "carla/rpc/ActorId.h"
#include "carla/geom/Vector3D.h"
#include "carla/sensor/RawData.h"

#include <cstdint>
#include <cstring>

namespace carla
{
namespace sensor
{

class SensorData;

namespace s11n
{

  class RaceLineSerializer
  {
  public:
    template <typename SensorT, typename EpisodeT, typename VectorT>
    static Buffer Serialize(
        const SensorT &,
        const EpisodeT &episode,
        const VectorT &data_vector)
    {
      log_info("RaceLineSerializer::Serialize");
      const uint32_t size_in_bytes = sizeof(geom::Vector3D) * data_vector.size();
      Buffer buffer{size_in_bytes};
      unsigned char *it = buffer.data();
      for (auto e : data_vector) {
        log_info("RaceLineSerializer::Serialize %f, %f, %f", e.x, e.y, e.z);
        std::memcpy(it, reinterpret_cast<const void *>(&e), sizeof(geom::Vector3D));
        it += sizeof(geom::Vector3D);
      }
      return buffer;
    }

    static SharedPtr<SensorData> Deserialize(RawData &&data);
  };

} // namespace s11n
} // namespace sensor
} // namespace carla
