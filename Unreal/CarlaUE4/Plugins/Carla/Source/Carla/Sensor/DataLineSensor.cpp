// Copyright (c) 2019 Intel Labs.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Carla.h"
#include "Carla/Sensor/DataLineSensor.h"

FActorDefinition ADataLineSensor::GetSensorDefinition()
{
  return UActorBlueprintFunctionLibrary::MakeGenericSensorDefinition(TEXT("other"), TEXT("data_line"));
}

ADataLineSensor::ADataLineSensor(const FObjectInitializer &ObjectInitializer)
  : Super(ObjectInitializer)
{
  PrimaryActorTick.bCanEverTick = false;

  spline.push_back(21);
  spline.push_back(22);
  spline.push_back(23);
  spline.push_back(24);
  for (auto &it : spline)
  {
    UE_LOG(LogTemp, Warning, TEXT("spline '%d'"), it);
  }

  raw_param = 2;
}

void ADataLineSensor::UpdateSpline(std::vector<int> &data)
{
  // Copy the data.
  spline = data;

  for (auto &it : spline)
  {
    UE_LOG(LogTemp, Warning, TEXT("spline '%d'"), it);
  }
}

void ADataLineSensor::UpdateRawParam(int &data)
{
  // Copy the data.
  raw_param = data;
  UE_LOG(LogTemp, Warning, TEXT("UpdateRawParam '%d'"), raw_param);
}
