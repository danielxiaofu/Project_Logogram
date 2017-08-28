// Fill out your copyright notice in the Description page of Project Settings.

#include "SketchingComponent.h"
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
	
}

// Called every frame
void USketchingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsSampling) 
	{
		SampleMouse();
	}
	// ...
}

void USketchingComponent::StartSample(FVector2D MousePos)
{
	// Check if canvas size is set properly
	if (CanvasSize == -1 || WayPointSampleDistance == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid CanvasSize or WayPointSampleDistance, set before drawing. CanvasSize = %f, WayPointSampleDistance = %f"), CanvasSize, WayPointSampleDistance)
		return;
	}

	if (!MouseOutOfRange())
	{
		// Add waypoint
		NewSymbol->AddStroke();
		NewSymbol->AddWaypoint(MousePos);
		LastSamplePoint = MousePos;

		// Add feature point
		NewSymbol->AddFeaturePoint(MousePos, 0);

		WaypointDelegate.Broadcast(MousePos);
	}
	
	bIsSampling = true;
}

void USketchingComponent::FinishSample(FVector2D MousePos)
{
	if (!MouseOutOfRange())
	{
		NewSymbol->AddWaypoint(MousePos);
		
		WaypointDelegate.Broadcast(MousePos);
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
	GenerateTurningPoint(0, NewSymbol->GetLastStroke().WayPoints.Num() - 1, 1);
	if (!MouseOutOfRange())
	{
		NewSymbol->AddFeaturePoint(MousePos, 0);
	}
	
	CalculateFeaturePointDirection();

}

TArray<FVector2D> USketchingComponent::GetLastWaypoints()
{
	return NewSymbol->GetLastWaypointArray();
}

TArray<FVector2D> USketchingComponent::GetLastFeaturePoints()
{
	return NewSymbol->GetLastFeaturePointArray();
}


void USketchingComponent::SampleMouse()
{
	if (MouseOutOfRange())
	{
		return;
	}

	float DistanceSquared = FVector2D::DistSquared(DecalPosition, LastSamplePoint);

	if (DistanceSquared > WayPointSampleDistance * WayPointSampleDistance)
	{
		NewSymbol->AddWaypoint(DecalPosition);

		/** If the distance is greater than twice of the sample distance, there may be a gap between two points,
		* therefore additional points should be added to the gap
		*/
		if (DistanceSquared > 4 * WayPointSampleDistance * WayPointSampleDistance)
		{
			FVector2D Direction = (DecalPosition - LastSamplePoint).GetSafeNormal();
			float Distance = FMath::Sqrt(DistanceSquared);
			int32 NumOfPoints = static_cast<int32>(Distance / WayPointSampleDistance);
			for (int32 i = 0; i < NumOfPoints; i++)
			{
				FVector2D NewPoint = LastSamplePoint + (Direction * WayPointSampleDistance);
				NewSymbol->AddWaypoint(NewPoint);
				LastSamplePoint = NewPoint;

				WaypointDelegate.Broadcast(NewPoint);
			}
		}

		LastSamplePoint = DecalPosition;
		WaypointDelegate.Broadcast(DecalPosition);

	}


}

void USketchingComponent::OnWhiteboardSpawned(float Size)
{
	NewSymbol = NewObject<USymbolCharacter>();

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
		NewSymbol->SetFeaturePointDirection(0, Angle);
		UE_LOG(LogTemp, Warning, TEXT("Start point direction = %f"), Angle)
	}

	for (int32 i = 1; i < NewSymbol->GetLastStroke().FeaturePoints.Num(); i++)
	{
		FVector2D Direction = NewSymbol->GetFeaturePointPosition(i) - NewSymbol->GetFeaturePointPosition(i - 1);
		float Angle = FMath::Atan2(Direction.Y, Direction.X);
		NewSymbol->SetFeaturePointDirection(i, Angle);
		int32 Pri = NewSymbol->GetLastStroke().FeaturePoints[i].Priority;
		UE_LOG(LogTemp, Warning, TEXT("Point direction = %f, Priority = %d"), Angle, Pri)
	}

}

bool USketchingComponent::MouseOutOfRange()
{
	return MousePosition.X < 0 || MousePosition.Y < 0;
}



