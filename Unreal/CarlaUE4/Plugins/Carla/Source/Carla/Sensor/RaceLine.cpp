// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Carla.h"
#include "Carla/Sensor/RaceLine.h"

#include "Carla/Actor/ActorBlueprintFunctionLibrary.h"
#include "Carla/Game/CarlaEpisode.h"
#include "Carla/Util/BoundingBoxCalculator.h"
#include "Carla/Vehicle/CarlaWheeledVehicle.h"

ARaceLine::ARaceLine(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer),
    UpdateAfterSeconds(100.0),
    ElapsedTimeSeconds(0.0)
{
  PrimaryActorTick.bCanEverTick = true;

  spline.push_back(carla::geom::Vector3D(1, 2, 1));
  spline.push_back(carla::geom::Vector3D(1, 2, 2));
  spline.push_back(carla::geom::Vector3D(1, 2, 3));
  spline.push_back(carla::geom::Vector3D(1, 2, 4));
  for (auto &it : spline)
  {
    UE_LOG(LogTemp, Warning, TEXT("Spline ('%f', '%f', '%f')"), it.x, it.y, it.z);
  }
}

FActorDefinition ARaceLine::GetSensorDefinition()
{
  auto Definition = UActorBlueprintFunctionLibrary::MakeGenericSensorDefinition(
      TEXT("other"),
      TEXT("race_line"));
  return Definition;
}

void ARaceLine::Set(const FActorDescription &Description)
{
  Super::Set(Description);
}

void ARaceLine::SetOwner(AActor *Owner)
{
  Super::SetOwner(Owner);
}

void ARaceLine::Tick(float DeltaSeconds)
{
  Super::Tick(DeltaSeconds);
  ElapsedTimeSeconds += DeltaSeconds;

  if (ElapsedTimeSeconds >= UpdateAfterSeconds)
  {
    // Update values.
    for (auto &it : spline)
    {
      it.x += 1;
      it.y += 1;
      it.z += 1;
    }

    // Reset time.
    ElapsedTimeSeconds = 0.0;

    // // Send Stream.
    auto Stream = GetDataStream(*this);
    Stream.Send(*this, GetEpisode(), spline);
    UE_LOG(LogTemp, Log, TEXT("Sent stream data!"));
  }
}

void ARaceLine::SetSpline(std::vector<carla::geom::Vector3D> &data)
{
  // Copy the data.
  spline = data;

  for (auto &it : spline)
  {
    UE_LOG(LogTemp, Log, TEXT("Spline ('%f', '%f', '%f')"), it.x, it.y, it.z);
  }
}

std::vector<carla::geom::Vector3D> ARaceLine::GetSpline()
{
  return spline;
}
