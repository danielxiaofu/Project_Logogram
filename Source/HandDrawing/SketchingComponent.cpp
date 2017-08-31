// Fill out your copyright notice in the Description page of Project Settings.

#include "SketchingComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Logogram/SymbolCharacter.h"

// Sets default values for this component's properties
USketchingComponent::USketchingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USketchingComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Board = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	check(Board && "Could not find board mesh");
	InitializeSampleDistance();

}

// Called every frame
void USketchingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void USketchingComponent::StartSample(FVector WorldBrushPos)
{
	if (bIsSampling)
		return;

	// Check if canvas size is set properly
	if (CanvasSize == -1 || WayPointSampleDistance == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid CanvasSize or WayPointSampleDistance, set before drawing. CanvasSize = %f, WayPointSampleDistance = %f"), CanvasSize, WayPointSampleDistance)
		return;
	}

	FVector2D BrushPos;
	ConvertWorldToLocalBoard(WorldBrushPos, BrushPos);

	if (!OutOfRange(WorldBrushPos))
	{
		// Add waypoint
		NewSymbol->AddStroke();
		NewSymbol->AddWaypoint(BrushPos);
		LastSamplePoint = BrushPos;

		// Add feature point
		NewSymbol->AddFeaturePoint(BrushPos, 0);

		WaypointDelegate.Broadcast(BrushPos);
	}
	
	bIsSampling = true;
}

void USketchingComponent::FinishSample(FVector WorldBrushPos)
{
	if (!bIsSampling)
		return;

	FVector2D BrushPos;
	ConvertWorldToLocalBoard(WorldBrushPos, BrushPos);

	if (!OutOfRange(WorldBrushPos))
	{
		NewSymbol->AddWaypoint(BrushPos);
		
		WaypointDelegate.Broadcast(BrushPos);
		// Debug, print all waypoints
		//for (FVector2D WayPoint : Symbol.Strokes.Last().WayPoints) 
		//{
		//	FString PositionString = WayPoint.ToString();
		//	
		//	//UE_LOG(LogTemp, Warning, TEXT("Final WayPoint: %s"), *PositionString)
		//}

		//UE_LOG(LogTemp, Warning, TEXT("Final Num of waypoints: %d"), Symbol.Strokes.Last().WayPoints.Num())
	}

	bIsSampling = false;

	// Generate turn point from the first point to the last point
	GenerateTurningPoint(0, NewSymbol->GetLastStroke().WayPoints.Num() - 1, 1);

	// Assign end point to the last feature point
	if (!OutOfRange(WorldBrushPos))
	{
		NewSymbol->AddFeaturePoint(BrushPos, 0);
	}
	
	CalculateFeaturePointDirection();

}

void USketchingComponent::ReqestToAddSample(FVector WorldBrushPos)
{
	if (!bIsSampling)
		return;

	if (OutOfRange(WorldBrushPos))
		return;

	FVector2D BrushPos;
	ConvertWorldToLocalBoard(WorldBrushPos, BrushPos);

	float DistanceSquared = FVector2D::DistSquared(BrushPos, LastSamplePoint);

	if (DistanceSquared > WayPointSampleDistance * WayPointSampleDistance)
	{
		NewSymbol->AddWaypoint(BrushPos);

		/** If the distance is greater than twice of the sample distance, there may be a gap between two points,
		* therefore additional points should be added to the gap
		*/
		if (DistanceSquared > 4 * WayPointSampleDistance * WayPointSampleDistance)
		{
			FVector2D Direction = (BrushPos - LastSamplePoint).GetSafeNormal();
			float Distance = FMath::Sqrt(DistanceSquared);
			int32 NumOfPoints = static_cast<int32>(Distance / WayPointSampleDistance);
			for (int32 i = 0; i < NumOfPoints; i++)
			{
				FVector2D NewPoint = LastSamplePoint + (Direction * WayPointSampleDistance);
				NewSymbol->AddWaypoint(NewPoint);
				LastSamplePoint = NewPoint;

				WaypointDelegate.Broadcast(NewPoint); // TODO Refact broadcasting system
			}
		}

		LastSamplePoint = BrushPos;
		WaypointDelegate.Broadcast(BrushPos);

	}


}

TArray<FVector2D> USketchingComponent::GetLastWaypoints()
{
	return NewSymbol->GetLastWaypointArray();
}

TArray<FVector2D> USketchingComponent::GetLastFeaturePoints()
{
	return NewSymbol->GetLastFeaturePointArray();
}

void USketchingComponent::InitializeSampleDistance()
{
	NewSymbol = NewObject<USymbolCharacter>();

	/* Calculate on-screen size of the board
	*/
	FVector2D ScreenBottomLeft, ScreenTopRight;
	PlayerController->ProjectWorldLocationToScreen(Board->GetSocketLocation(FName("BottomLeft")), ScreenBottomLeft);
	PlayerController->ProjectWorldLocationToScreen(Board->GetSocketLocation(FName("TopRight")), ScreenTopRight);
	FVector2D Diagonal = ScreenTopRight - ScreenBottomLeft;
	float Size = FMath::Abs(Diagonal.X);
	CanvasSize = Size;

	WayPointSampleDistance = CanvasSize / MaxSamplesAcrossScreen;

	//UE_LOG(LogTemp, Warning, TEXT("Canvas size = %f"), CanvasSize)
}

bool USketchingComponent::GenerateTurningPoint(int32 PointIndex1, int32 PointIndex2, int32 Priority)
{
	if (PointIndex1 < 0 || PointIndex2 <= 0 || PointIndex1 >= PointIndex2 - 1)
	{
		return false;
	}

	const float A = NewSymbol->GetWaypoint(PointIndex2).X - NewSymbol->GetWaypoint(PointIndex1).X;
	const float B = NewSymbol->GetWaypoint(PointIndex2).Y - NewSymbol->GetWaypoint(PointIndex1).Y;
	const float C = NewSymbol->GetWaypoint(PointIndex1).X * NewSymbol->GetWaypoint(PointIndex2).Y - NewSymbol->GetWaypoint(PointIndex2).X * NewSymbol->GetWaypoint(PointIndex1).Y;
	float Max = 3000;
	int32 MaxDistPointIndex = -1;
	for (int32 i = PointIndex1 + 1; i < PointIndex2; i++) 
	{
		FVector2D Point = NewSymbol->GetWaypoint(i);
		//const float Distance = FMath::Abs((A * Point.Y) - (B * Point.X) + C) / (FMath::Square(B) + FMath::Square(A));
		const float Distance = FMath::Abs((A * Point.Y) - (B * Point.X) + C);
		//UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), Distance)
		if (Distance > Max)
		{
			Max = Distance;
			MaxDistPointIndex = i;
			
		}
	}
	// Recursively look for other way points
	if (MaxDistPointIndex != -1) {
		GenerateTurningPoint(PointIndex1, MaxDistPointIndex, Priority + 1);
		NewSymbol->AddFeaturePoint(NewSymbol->GetWaypoint(MaxDistPointIndex), Priority);
		//UE_LOG(LogTemp, Warning, TEXT("Turn point created, max distance = %f"), Max)
		GenerateTurningPoint(MaxDistPointIndex, PointIndex2, Priority + 1);
		return true;
	}

	return false;
}

void USketchingComponent::CalculateFeaturePointDirection() 
{

	if (NewSymbol->NumOfStroke() == 0)
	{
		return;
	}
	// If current stroke is the first stroke, set the direction of the first feature point to 0
	else if (NewSymbol->NumOfStroke() == 1)
	{
		NewSymbol->SetFeaturePointDirection(0, 0);
		UE_LOG(LogTemp, Warning, TEXT("Start point direction = %f"), 0)
	}
	// Else the direction of the start point will be relative to the end point of last stroke
	else 
	{
		FVector2D LastEndPoint = NewSymbol->GetStroke(NewSymbol->NumOfStroke() - 2).FeaturePoints.Last().Position;
		FVector2D Direction = NewSymbol->GetFeaturePointPosition(0) - LastEndPoint;
		float Angle = FMath::Atan2(Direction.Y, Direction.X);
		if (Angle < 0)
			Angle = PI * 2 + Angle;
		NewSymbol->SetFeaturePointDirection(0, Angle);
		UE_LOG(LogTemp, Warning, TEXT("Start point direction = %f"), Angle)
	}

	for (int32 i = 1; i < NewSymbol->GetLastStroke().FeaturePoints.Num(); i++)
	{
		FVector2D Direction = NewSymbol->GetFeaturePointPosition(i) - NewSymbol->GetFeaturePointPosition(i - 1);
		float Angle = FMath::Atan2(Direction.Y, Direction.X);
		if (Angle < 0)
			Angle = PI * 2 + Angle;
		NewSymbol->SetFeaturePointDirection(i, Angle);
		int32 Pri = NewSymbol->GetLastStroke().FeaturePoints[i].Priority;
		UE_LOG(LogTemp, Warning, TEXT("Point direction = %f, Priority = %d"), Angle, Pri)
	}

}

void USketchingComponent::ConvertWorldToLocalBoard(FVector WorldPos, FVector2D& LocalPos)
{
	FVector2D ScreenBottomLeft;
	PlayerController->ProjectWorldLocationToScreen(Board->GetSocketLocation(FName("BottomLeft")), ScreenBottomLeft);
	FVector2D ScreenPosition;
	PlayerController->ProjectWorldLocationToScreen(WorldPos, ScreenPosition);
	FVector2D LocalPosition = ScreenPosition - ScreenBottomLeft;
	LocalPos.X = LocalPosition.X;
	LocalPos.Y = LocalPosition.Y;
}

void USketchingComponent::BroadCastLocalToWorld(FVector2D & LocalPos)
{

}

bool USketchingComponent::OutOfRange(FVector WorldPos)
{
	FVector2D LocalPosition;
	ConvertWorldToLocalBoard(WorldPos, LocalPosition);
	return LocalPosition.X < 0 || LocalPosition.Y < 0 || LocalPosition.X > CanvasSize || LocalPosition.Y > CanvasSize;
}



