// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <vector>

#include "Carla/Actor/ActorDefinition.h"
#include "Carla/Actor/ActorDescription.h"
#include "Carla/Sensor/Sensor.h"

#include <carla/geom/Vector3D.h>

#include <Components/SplineComponent.h>
#include <Components/HierarchicalInstancedStaticMeshComponent.h>

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

  // Spline points
  void SetSpline(std::vector<carla::geom::Vector3D> &data);
  std::vector<carla::geom::Vector3D> GetSpline();

  void CalculateArrowLength();
  void AddArrowsToSpline();
  void ColorArrows();
  float GetColorWeightAtLocation(FVector Location);

  // // NOTE: Might not be needed.
  // void SetNumberOfElementsInColorArray();


private:

  // --------------------------------------------------------------------------
  // Editable components from the PythonAPI.
  // --------------------------------------------------------------------------
  std::vector<carla::geom::Vector3D> SplinePoints;
  std::vector<float> ColorArrayDetail;

  UPROPERTY(EditAnywhere)
  float UpdateAfterSeconds;

  UPROPERTY(VisibleAnywhere)
  float ElapsedTimeSeconds;

  // --------------------------------------------------------------------------
  // Race Line Properties.
  // NOTE: Maybe editable?
  // --------------------------------------------------------------------------
  // Toe-to-Tip offset between each arrow.
  UPROPERTY(EditAnywhere)
  float ArrowHorizontalOffset;

  // Vertical offset from spline.
  UPROPERTY(EditAnywhere)
  float ArrowVerticalOffset;

  // --------------------------------------------------------------------------
  // Race Line Attributes.
  // --------------------------------------------------------------------------
  UPROPERTY(VisibleAnywhere)
  USplineComponent* Spline;

  UPROPERTY(VisibleAnywhere)
  UHierarchicalInstancedStaticMeshComponent* SplineHISM;

  UPROPERTY(VisibleAnywhere)
  TArray<float> ColorArray;

  // Holds the arrow static mesh.
  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* ArrowMesh;

  // Arrow Length.
  UPROPERTY(VisibleAnywhere)
  float ArrowLength;

  // Number of Arrows.
  UPROPERTY(VisibleAnywhere)
  int NumberOfArrows;
};
