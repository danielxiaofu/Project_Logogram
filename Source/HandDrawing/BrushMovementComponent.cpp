// Fill out your copyright notice in the Description page of Project Settings.

#include "BrushMovementComponent.h"


// Sets default values for this component's properties
UBrushMovementComponent::UBrushMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrushMovementComponent::BeginPlay()
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
void UBrushMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	UpdateBrushPosition(DeltaTime);
}

void UBrushMovementComponent::SetBoardAxis(FVector AxisX, FVector AxisY, FVector BottomLeft, FVector TopRight)
{
	BoardAxisX = AxisX;
	BoardAxisY = AxisY;
	Origin = BottomLeft;
	TopRightOrigin = TopRight;
}

void UBrushMovementComponent::UpdateBrushPosition(float DeltaTime)
{
	if (BrushScreenVelocity.SizeSquared() == 0)
		return;

	if (BoardAxisX.IsZero() || BoardAxisY.IsZero())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to update brush position because board axis is zero vector, call SetBoardAxis before updating"))
		return;
	}

	FVector2D Direction = GetConstrainDirection(BrushScreenVelocity.GetSafeNormal());
	FVector WorldDirection = BoardAxisX * Direction.X + BoardAxisY * Direction.Y;

	FVector FinalDirection = BrushScreenVelocity.SizeSquared() > 1 ? WorldDirection.GetSafeNormal() : WorldDirection.GetSafeNormal() * BrushScreenVelocity.Size();
	FVector FinalPosition = BrushPosition + FinalDirection * BrushSpeed * DeltaTime;

	// check if brush is out of range
	float Dot1 = FVector::DotProduct((FinalPosition - Origin).GetSafeNormal(), BoardAxisX);
	float Dot2 = FVector::DotProduct((FinalPosition - Origin).GetSafeNormal(), BoardAxisY);
	float Dot3 = FVector::DotProduct((FinalPosition - TopRightOrigin).GetSafeNormal(), -BoardAxisX);
	float Dot4 = FVector::DotProduct((FinalPosition - TopRightOrigin).GetSafeNormal(), -BoardAxisY);
	if (Dot1 >= 0 && Dot2 >= 0 && Dot3 >= 0 && Dot4 >= 0)
		BrushPosition = FinalPosition;

	BrushPositionDelegate.Broadcast(BrushPosition);
}

FVector2D UBrushMovementComponent::GetConstrainDirection(FVector2D RawDirection) const
{
	FVector2D Direction = RawDirection.GetSafeNormal();
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

