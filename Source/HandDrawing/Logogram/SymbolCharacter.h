// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SymbolCharacter.generated.h"

/** Feature point stands for two kind of special points in a stroke:
* 1. Turning point, the point where the direction of the stroke changes.
* 2. End point, including start and end point of the stroke
*/
USTRUCT(BlueprintType)
struct FFeaturePoint
{
	GENERATED_BODY()

		FFeaturePoint()
	{
		Position = FVector2D(0, 0);
		Direction = -1;
		Priority = 0;
	}

	FFeaturePoint(FVector2D Vector, int32 PriorityLevel)
	{
		Position = Vector;
		Direction = -1;
		Priority = PriorityLevel;
	}

	// Local position of the point (relative to drawing frame) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature Point")
		FVector2D Position;

	// Angle of the line connecting this point and last feature point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature Point")
		float Direction;

	// Order of the point when it is generated, start&end points have Priority 0, first generated turnpoint has priority 1, etc..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature Point")
		int32 Priority;

};

USTRUCT(BlueprintType)
struct FStroke
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stroke")
	TArray<FVector2D> WayPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stroke")
		TArray<FFeaturePoint> FeaturePoints;

	FVector2D& GetWaypoint(int32 Index) {
		if (Index >= WayPoints.Num())
		{
			FVector2D Zero = FVector2D(0, 0);
			return Zero;
		}
		return WayPoints[Index];
	}

};

/** A Symbol stores all strokes and waypoints that user created on the whiteboard
 * 
 */
UCLASS()
class HANDDRAWING_API USymbolCharacter : public UObject
{
	GENERATED_BODY()
	
private:
	TArray<FStroke> Strokes;

	FStroke InvalidStroke;

public:
	FStroke& GetStroke(int32 Index);

	FStroke& GetLastStroke();

	TArray<FVector2D> GetLastWaypointArray();

	TArray<FVector2D> GetLastFeaturePointArray();

	int32 NumOfStroke();

	void AddStroke();

	bool AddWaypoint(FVector2D WayPoint);

	bool AddFeaturePoint(FVector2D Point, int32 Priority);

	/** Get the waypoint on the newest stroke
	* @param Index index of the waypoint in the array
	* @return reference of the waypoint
	*/
	FVector2D& GetWaypoint(int32 Index);

	/** Get the position of feature point on the newest stroke
	* @param Index index of the feature point in the array
	* @return reference of the position
	*/
	FVector2D& GetFeaturePointPosition(int32 Index);

	/** Set the direction of feature point on the newest stroke
	* @param Index index of the feature point in the array
	* @param Direction direction to be set
	* @return reference of the position
	*/
	bool SetFeaturePointDirection(int32 Index, float Direction);
	
	
};
