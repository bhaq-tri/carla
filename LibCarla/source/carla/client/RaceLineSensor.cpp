// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
// Description:
//    This file is the Client Actor. It can communicate with the Blueprint
//    actor as defined in the `Plugins`.


#include "carla/client/RaceLineSensor.h"

#include "carla/Logging.h"
#include "carla/client/Map.h"
#include "carla/client/Vehicle.h"
#include "carla/client/detail/Simulator.h"
#include "carla/geom/Location.h"
#include "carla/geom/Math.h"
#include "carla/sensor/data/RaceLineSensorEvent.h"
#include "carla/sensor/data/LaneInvasionEvent.h"

#include <exception>

namespace carla {
namespace client {

void RaceLineSensor::UpdateSpline(std::vector<geom::Vector3D> &data)
{
  // Copy the data.
  log_info("RaceLineSensor::UpdateSpline");
  GetEpisode().Lock()->UpdateSpline(*this, data);
}

void RaceLineSensor::SetColor(std::vector<float> &data)
{
  // Copy the data.
  log_info("RaceLineSensor::SetColor");
  GetEpisode().Lock()->SetColor(*this, data);
}

void RaceLineSensor::SetArrowHorizontalOffset(float &data)
{
  // Copy the data.
  log_info("RaceLineSensor::SetArrowHorizontalOffset");
  GetEpisode().Lock()->SetArrowHorizontalOffset(*this, data);
}

void RaceLineSensor::SetArrowVerticalOffset(float &data)
{
  // Copy the data.
  log_info("RaceLineSensor::SetArrowVerticalOffset");
  GetEpisode().Lock()->SetArrowVerticalOffset(*this, data);
}

RaceLineSensor::~RaceLineSensor() {
  Stop();
}

void RaceLineSensor::Listen(CallbackFunctionType callback) {
  log_debug(GetDisplayId(), ": subscribing to stream");
  GetEpisode().Lock()->SubscribeToSensor(*this, std::move(callback));
  _is_listening = true;
}

void RaceLineSensor::Stop() {
  if (!_is_listening) {
    log_warning(
        "attempting to unsubscribe from stream but sensor wasn't listening:",
        GetDisplayId());
    return;
  }
  GetEpisode().Lock()->UnSubscribeFromSensor(*this);
  _is_listening = false;
}

bool RaceLineSensor::Destroy() {
  if (IsListening()) {
    Stop();
  }
  return Actor::Destroy();
}

} // namespace client
} // namespace carla
