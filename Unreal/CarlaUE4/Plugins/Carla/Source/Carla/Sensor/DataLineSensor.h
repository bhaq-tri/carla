// Copyright (c) 2019 Intel Labs.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <vector>

#include "Carla/Sensor/ShaderBasedSensor.h"

#include "Carla/Actor/ActorDefinition.h"

#include "DataLineSensor.generated.h"

/// LaneInvasion sensor representation
/// The actual position calculation is done one client side
UCLASS()
class CARLA_API ADataLineSensor : public ASensor
{
  GENERATED_BODY()

public:

  static FActorDefinition GetSensorDefinition();

  ADataLineSensor(const FObjectInitializer &ObjectInitializer);

  void UpdateSpline(std::vector<int> &data);

  std::vector<int> spline;

  UPROPERTY(EditAnywhere)
  int raw_param;

  void UpdateRawParam(int &data);
};
