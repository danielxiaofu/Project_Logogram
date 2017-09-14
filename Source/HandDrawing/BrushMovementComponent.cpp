// Fill out your copyright notice in the Description page of Project Settings.

#include "BrushMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

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
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

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

void UBrushMovementComponent::SetBoardAxis(FVector AxisX, FVector AxisY, FVector BottomLeft, FVector TopRight, FVector TopLeft)
{
	BoardAxisX = AxisX;
	BoardAxisY = AxisY;
	Origin = BottomLeft;
	TopRightCorner = TopRight;
	TopLeftCorner = TopLeft;
}

void UBrushMovementComponent::SetEightDirectionMode(bool bEightDirection)
{
	bEightDirectionMode = bEightDirection;
}

void UBrushMovementComponent::SetDirectMouseControl(bool bMouseControl)
{
	bDirectMouseControl = bMouseControl;
}

void UBrushMovementComponent::UpdateBrushPosition(float DeltaTime)
{

	if (BoardAxisX.IsZero() || BoardAxisY.IsZero())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to update brush position because board axis is zero vector, call SetBoardAxis before updating"))
		return;
	}

	if (!bDirectMouseControl)
	{
		if (BrushScreenVelocity.SizeSquared() != 0)
		{
			FVector2D Direction;
			if (bEightDirectionMode)
				Direction = GetConstrainDirection(BrushScreenVelocity.GetSafeNormal());
			else
				Direction = BrushScreenVelocity.GetSafeNormal();

			FVector WorldDirection = BoardAxisX * Direction.X + BoardAxisY * Direction.Y;

			FVector FinalDirection = BrushScreenVelocity.SizeSquared() > 1 ? WorldDirection.GetSafeNormal() : WorldDirection.GetSafeNormal() * BrushScreenVelocity.Size();
			FVector FinalPosition = BrushPosition + FinalDirection * BrushSpeed * DeltaTime;

			// check if brush is out of range
			float Dot1 = FVector::DotProduct((FinalPosition - Origin).GetSafeNormal(), BoardAxisX);
			float Dot2 = FVector::DotProduct((FinalPosition - Origin).GetSafeNormal(), BoardAxisY);
			float Dot3 = FVector::DotProduct((FinalPosition - TopRightCorner).GetSafeNormal(), -BoardAxisX);
			float Dot4 = FVector::DotProduct((FinalPosition - TopRightCorner).GetSafeNormal(), -BoardAxisY);
			if (Dot1 >= 0 && Dot2 >= 0 && Dot3 >= 0 && Dot4 >= 0)
				BrushPosition = FinalPosition;
		}
	}
	else
	{

		float MouseX = 0;
		float MouseY = 0;
		PlayerController->GetMousePosition(MouseX, MouseY);
		FVector2D MousePosition = FVector2D(MouseX, MouseY);

		UE_LOG(LogTemp, Warning, TEXT("MousePosition: %s"), *(MousePosition.ToString()))

		// Check if mouse is inside the board
		FVector2D BottomLeft = FVector2D(0, 0);
		FVector2D TopRight = FVector2D(0, 0);
		PlayerController->ProjectWorldLocationToScreen(Origin, BottomLeft);
		PlayerController->ProjectWorldLocationToScreen(TopRightCorner, TopRight);
		if (MousePosition.X > BottomLeft.X && MousePosition.X < TopRight.X && MousePosition.Y < BottomLeft.Y && MousePosition.Y > TopRight.Y)
		{
			// Get mouse local position on board
			FVector2D MouseLocal = MousePosition - BottomLeft;
			MouseLocal.Y *= -1;
			// Change to scale based position
			float BoardSize = FMath::Abs(TopRight.X - BottomLeft.X);
			float ScaleX = FMath::Abs(MouseLocal.X / BoardSize);
			float ScaleY = FMath::Abs(MouseLocal.Y / BoardSize);

			// Use the scale based position to find world position
			
			FVector WorldOnBoard = (TopRightCorner - TopLeftCorner) * ScaleX + (TopLeftCorner - Origin) * ScaleY;
			BrushPosition = WorldOnBoard + Origin;
		}

	}

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

