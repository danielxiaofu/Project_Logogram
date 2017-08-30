// Fill out your copyright notice in the Description page of Project Settings.

#include "BrushConstrain.h"


// Sets default values for this component's properties
UBrushConstrain::UBrushConstrain()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrushConstrain::BeginPlay()
{
	Super::BeginPlay();

	// ...
	North = FVector2D(0, 1);
	South = FVector2D(0, -1);
	East = FVector2D(1, 0);
	West = FVector2D(-1, 0);

	NorthEast = (North + East).GetSafeNormal();
	NorthWest = (North + West).GetSafeNormal();
	SouthEast = (South + East).GetSafeNormal();
	SouthWest = (South + West).GetSafeNormal();
}


// Called every frame
void UBrushConstrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector2D UBrushConstrain::GetConstrainDirection(FVector2D BrushPosition, FVector2D MousePosition) const
{
	FVector2D Direction = MousePosition - BrushPosition;
	float Angle = FMath::Atan2(Direction.Y, Direction.X);
	float Half45 = PI / 8;
	
	if (-Half45 <= Angle && Angle < Half45)
		Direction = East;
	else if (Half45 <= Angle && Angle < 3 * Half45)
		Direction = NorthEast;
	else if (3 * Half45 <= Angle && Angle < 5 * Half45)
		Direction = North;
	else if (5 * Half45 <= Angle && Angle < 7 * Half45)
		Direction = NorthWest;
	else if ((7 * Half45 <= Angle && Angle <= 8 * Half45) || (-8 * Half45 <= Angle && Angle < -7 * Half45))
		Direction = West;
	else if (-7 * Half45 <= Angle && Angle < -5 * Half45)
		Direction = SouthWest;
	else if (-5 * Half45 <= Angle && Angle < -3 * Half45)
		Direction = South;
	else
		Direction = SouthEast;

	return Direction.GetSafeNormal();
}


