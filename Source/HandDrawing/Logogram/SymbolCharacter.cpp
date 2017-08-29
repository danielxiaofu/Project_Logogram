// Fill out your copyright notice in the Description page of Project Settings.


#include "SymbolCharacter.h"

FStroke & USymbolCharacter::GetStroke(int32 Index)
{
	if (Index >= Strokes.Num())
	{
		return InvalidStroke;
	}

	return Strokes[Index];
}

FStroke & USymbolCharacter::GetLastStroke()
{
	return Strokes.Last();
}

TArray<FVector2D> USymbolCharacter::GetLastWaypointArray()
{
	if (Strokes.Num() <= 0)
	{
		return TArray<FVector2D>();
	}

	return Strokes.Last().WayPoints;
}

TArray<FVector2D> USymbolCharacter::GetLastFeaturePointArray()
{
	if (Strokes.Num() <= 0)
	{
		return TArray<FVector2D>();
	}

	TArray<FVector2D> Points;

	for (int32 i = 0; i < Strokes.Last().FeaturePoints.Num(); i++)
	{
		FFeaturePoint Point = Strokes.Last().FeaturePoints[i];
		Points.Add(Point.Position);
	}

	return Points;
}

int32 USymbolCharacter::NumOfStroke()
{
	return Strokes.Num();
}

void USymbolCharacter::AddStroke()
{
	Strokes.Add(FStroke());
}

bool USymbolCharacter::AddWaypoint(FVector2D WayPoint)
{
	if (Strokes.Num() == 0)
	{
		return false;
	}
	Strokes.Last().WayPoints.Add(WayPoint);
	return true;
}

bool USymbolCharacter::AddFeaturePoint(FVector2D Point, int32 Priority)
{
	if (Strokes.Num() == 0)
	{
		return false;
	}
	Strokes.Last().FeaturePoints.Add(FFeaturePoint(Point, Priority));
	return true;
}

FVector2D& USymbolCharacter::GetWaypoint(int32 Index)
{
	return Strokes.Last().WayPoints[Index];
}

FVector2D& USymbolCharacter::GetFeaturePointPosition(int32 Index)
{
	return Strokes.Last().FeaturePoints[Index].Position;
}

bool USymbolCharacter::SetFeaturePointDirection(int32 Index, float Direction)
{
	if (Index < 0 || Index >= Strokes.Last().FeaturePoints.Num())
	{
		return false;
	}

	Strokes.Last().FeaturePoints[Index].Direction = Direction;
	return true;
}


