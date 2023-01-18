// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <vector>

#include "Carla/Sensor/Sensor.h"

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"

#include <Components/BoxComponent.h>
#include <carla/geom/Vector3D.h>

#include "RaceLine.generated.h"

UCLASS()
class CARLA_API ARaceLine : public ASensor
{
  GENERATED_BODY()

public:
  ARaceLine(const FObjectInitializer &ObjectInitializer);

  static FActorDefinition GetSensorDefinition();

  void Set(const FActorDescription &ActorDescription) override;

  void SetOwner(AActor *Owner) override;

  void Tick(float DeltaSeconds) override;

  void SetSpline(std::vector<carla::geom::Vector3D> &data);
  std::vector<carla::geom::Vector3D> GetSpline();

private:
  std::vector<carla::geom::Vector3D> spline;

  UPROPERTY(EditAnywhere)
  float UpdateAfterSeconds;

  UPROPERTY(VisibleAnywhere)
  float ElapsedTimeSeconds;
};
