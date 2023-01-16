// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/client/ClientSideSensor.h"
#include "carla/geom/Vector3D.h"

#include <atomic>
#include <vector>

namespace carla {
namespace client {

  class Map;
  class Vehicle;

  class RaceLineSensor final : public Sensor {
  public:

    using Sensor::Sensor;

    ~RaceLineSensor();

    /// Register a @a callback to be executed each time a new measurement is
    /// received.
    ///
    /// @warning Calling this function on a sensor that is already listening
    /// steals the data stream from the previously set callback. Note that
    /// several instances of Sensor (even in different processes) may point to
    /// the same sensor in the simulator.
    void Listen(CallbackFunctionType callback) override;

    /// Stop listening for new measurements.
    void Stop() override;

    /// Return whether this Sensor instance is currently listening to the
    /// associated sensor in the simulator.
    bool IsListening() const override {
      return _is_listening;
    }

    void UpdateSpline(std::vector<geom::Vector3D> &data);

    bool Destroy() override;

  private:

    bool _is_listening = false;

  };

} // namespace client
} // namespace carla
