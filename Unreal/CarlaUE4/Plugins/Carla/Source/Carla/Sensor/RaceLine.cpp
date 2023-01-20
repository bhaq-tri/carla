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

#include "Engine/StaticMesh.h"
#include "Math/Transform.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ARaceLine::ARaceLine(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer),
    UpdateAfterSeconds(100.0),
    ElapsedTimeSeconds(0.0)
{
  PrimaryActorTick.bCanEverTick = true;

  // Setup Spline
  Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
  // Spline->SetupAttachment(RootComponent);
  RootComponent = Spline;
  Spline->bDrawDebug = true;

  // Setup the arrow mesh.
  // TODO: Make less brittle?
  ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Carla/Blueprints/RaceLine/M_Arrow.M_Arrow'"));
  ArrowMesh->SetStaticMesh(MeshAsset.Object);

  SplineHISM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("SplineHISM"));
  SplineHISM->SetupAttachment(Spline);

  // Set the arrow mesh
  SplineHISM->SetStaticMesh(ArrowMesh->GetStaticMesh());
  // Needed to tell the HISM about the custom data field used to color the arrows.
  SplineHISM->NumCustomDataFloats = 1;
  ArrowVerticalOffset = 50;
  ArrowHorizontalOffset = 120;
  // Debug tasks.
  SplinePoints.push_back(carla::geom::Vector3D(1000, -400, 0));
  SplinePoints.push_back(carla::geom::Vector3D(4000, -200, 0));
  SplinePoints.push_back(carla::geom::Vector3D(3000,  400, 0));
  SplinePoints.push_back(carla::geom::Vector3D(1500,  600, 0));
  SplinePoints.push_back(carla::geom::Vector3D(3000,  800, 0));
  ColorArrayDetail.push_back(1.0);
  ColorArrayDetail.push_back(0.0);
  ColorArrayDetail.push_back(1.0);
  ColorArrayDetail.push_back(0.0);
  ColorArrayDetail.push_back(1.0);

  FVector routePoint{0.0, 0.0 ,0.0};
  for (auto &it : SplinePoints)
  {
    UE_LOG(LogTemp, Warning, TEXT("Adding SplinePoints ('%f', '%f', '%f')"), it.x, it.y, it.z);
    routePoint[0] = it.x;
    routePoint[1] = it.y;
    routePoint[2] = it.z;
    Spline->AddSplinePoint(routePoint, ESplineCoordinateSpace::World, true);
  }

  UpdateAndDraw();
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
    for (auto &it : SplinePoints)
    {
      it.x += 1;
      it.y += 1;
      it.z += 1;
    }

    // Reset time.
    ElapsedTimeSeconds = 0.0;

    // // Send Stream.
    auto Stream = GetDataStream(*this);
    Stream.Send(*this, GetEpisode(), SplinePoints);
    UE_LOG(LogTemp, Log, TEXT("Sent stream data!"));
  }
}

// --------------------------------------------------------------------------
// Setters.
// --------------------------------------------------------------------------
void ARaceLine::SetSpline(std::vector<carla::geom::Vector3D> &data)
{
  // Copy the data.
  SplinePoints = data;

  // Create a new Spline component.
  USplineComponent* NewSpline = NewObject<USplineComponent>(this);
  FVector routePoint{0.0, 0.0 ,0.0};

  Spline->ClearSplinePoints();
  for (auto &it : SplinePoints)
  {
    UE_LOG(LogTemp, Log, TEXT("SplinePoints ('%f', '%f', '%f')"), it.x, it.y, it.z);
    routePoint[0] = it.x;
    routePoint[1] = it.y;
    routePoint[2] = it.z;
    Spline->AddSplinePoint(routePoint, ESplineCoordinateSpace::World, true);
  }

  // Update the Spline component.
  UpdateAndDraw();
}

void ARaceLine::SetColor(std::vector<float> &data)
{
  // Copy the data.
  ColorArrayDetail = data;
  UpdateAndDraw();
  UE_LOG(LogTemp, Log, TEXT("SetColor"));
}

void ARaceLine::SetArrowHorizontalOffset(float &data)
{
  ArrowHorizontalOffset = data;
  UpdateAndDraw();
}

void ARaceLine::SetArrowVerticalOffset(float &data)
{
  ArrowVerticalOffset = data;
  UpdateAndDraw();
}

std::vector<carla::geom::Vector3D> ARaceLine::GetSpline()
{
  return SplinePoints;
}

// --------------------------------------------------------------------------
// Render Arrows.
// --------------------------------------------------------------------------
void ARaceLine::UpdateAndDraw()
{
  if (SplinePoints.size() != ColorArrayDetail.size())
  {
    UE_LOG(LogTemp, Warning, TEXT("SplinePoints.size(%d) != ColorArrayDetail.size(%d), try again!"), SplinePoints.size(), ColorArrayDetail.size());
    return;
  }
  CalculateArrowLength();
  AddArrowsToSpline();
  ColorArrows();
}

void ARaceLine::CalculateArrowLength()
{
  if (ArrowMesh == nullptr)
  {
    UE_LOG(LogTemp, Warning, TEXT("CalculateArrowLength() failed: ArrowMesh is nullptr"));
    return;
  }

  FVector MeshDimension = ArrowMesh->GetStaticMesh()->GetBoundingBox().Max - ArrowMesh->GetStaticMesh()->GetBoundingBox().Min;
  ArrowLength = MeshDimension[0] + ArrowHorizontalOffset;
  UE_LOG(LogTemp, Log, TEXT("CalculateArrowLength() = %f"), ArrowLength);
}

void ARaceLine::AddArrowsToSpline()
{
  if (ArrowMesh == nullptr)
  {
    UE_LOG(LogTemp, Warning, TEXT("AddArrowsToSpline() failed: ArrowMesh is nullptr"));
    return;
  }

  SplineHISM->ClearInstances();

  // Fill the HISM.
  NumberOfArrows = FMath::FloorToInt(Spline->GetSplineLength()/ArrowLength);
  FVector CurrentLocation = FVector::ZeroVector;
  FVector NextLocation = FVector::ZeroVector;
  FTransform InstanceTransform = FTransform(CurrentLocation);

  InstanceTransform.SetScale3D(FVector::OneVector);

  for (int idx = 0; idx < NumberOfArrows; idx++)
  {
    CurrentLocation = Spline->GetLocationAtDistanceAlongSpline(ArrowLength * idx, ESplineCoordinateSpace::Local);
    NextLocation = Spline->GetLocationAtDistanceAlongSpline(ArrowLength * (idx+1), ESplineCoordinateSpace::Local);

    // Set translation with the vertical offset.
    CurrentLocation.Z += ArrowVerticalOffset;
    InstanceTransform.SetTranslation(CurrentLocation);

    InstanceTransform.SetRotation(FRotationMatrix::MakeFromX(NextLocation - CurrentLocation).ToQuat());

    SplineHISM->AddInstance(InstanceTransform);
  }

  // SplineHISM->;
}

float ARaceLine::GetColorWeightAtLocation(FVector Location)
{
  int P1, P2;
  float Weight, DifferenceWeight, FinalWeight, InputKey;

  InputKey = Spline->FindInputKeyClosestToWorldLocation(Location);
  P1 = FMath::FloorToInt(InputKey);
  P2 = P1 + 1;
  Weight = FMath::Fmod(InputKey, 1.0);
  DifferenceWeight = ColorArrayDetail[P2] - ColorArrayDetail[P1];
  FinalWeight = Weight * DifferenceWeight + ColorArrayDetail[P1];
  // UE_LOG(LogTemp, Log, TEXT("GetColorWeightAtLocation() P1=%d, InputKey=%f, Weight=%f, FinalWeight=%f "), P1, InputKey, Weight, FinalWeight);
  return FinalWeight;
}

void ARaceLine::ColorArrows()
{
  FVector Location;
  float ColorWeight = 0.0;
  bool IsValueSet = false;
  for (int idx = 0; idx < NumberOfArrows; idx++)
  {
    Location = Spline->GetLocationAtDistanceAlongSpline(ArrowLength * idx, ESplineCoordinateSpace::World);
    ColorWeight = GetColorWeightAtLocation(Location);
    IsValueSet = SplineHISM->SetCustomDataValue(idx, 0, ColorWeight, false);

    // Debug some shit.
    UE_LOG(LogTemp, Log, TEXT("PerInstanceSMData.IsValidIndex(%d) = %d"), idx, SplineHISM->PerInstanceSMData.IsValidIndex(idx));
  }
}
