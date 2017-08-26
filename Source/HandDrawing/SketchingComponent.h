// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SketchingComponent.generated.h"

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

	void CalculateDirection(int32 PointIndex) {

	}

};

USTRUCT(BlueprintType)
struct FSymbol
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FStroke> Strokes;

	void AddStroke(FStroke NewStroke) {
		Strokes.Add(NewStroke);
	}

	bool AddWayPoint(FVector2D WayPoint) {
		if (Strokes.Num() == 0)
		{
			return false;
		}
		Strokes.Last().WayPoints.Add(WayPoint);
		return true;
	}

	bool AddFeaturePoint(FVector2D Point, int32 Priority) {
		if (Strokes.Num() == 0)
		{
			return false;
		}
		Strokes.Last().FeaturePoints.Add(FFeaturePoint(Point, Priority));
		return true;
	}

	/** Get the waypoint on the newest stroke
    * @param Index index of the waypoint in the array
	* @return reference of the waypoint
	*/
	FVector2D& GetWaypoint(int32 Index) {

		return Strokes.Last().WayPoints[Index];
	}

	FVector2D& GetFeaturePointPosition(int32 Index) {
		return Strokes.Last().FeaturePoints[Index].Position;
	}

	bool SetFeaturePointDirection(int32 Index, float Direction) {
		if (Index < 0 || Index >= Strokes.Last().FeaturePoints.Num())
		{
			return false;
		}

		Strokes.Last().FeaturePoints[Index].Direction = Direction;
		return true;
	}
};

/** This component is responsible for recieving mouse positions while drawing and use the position 
  * data to construct way points along the stroke. It also recognize turning points that will be useful
  * for symbol recognition.
  * Note: All calculations of mouse and point positions in this component are in the drawing area's local
  * coordinates, with the bottom left corner being origin(0,0).
  */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANDDRAWING_API USketchingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVectorDelegate, const FVector2D&, Position);

	USketchingComponent();


	// Fired when a waypoint is added to the current stroke
	UPROPERTY(BlueprintAssignable, Category = "Drawing")
	FVectorDelegate WaypointDelegate;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MousePosition;

	UPROPERTY(BlueprintReadWrite)
	FVector2D DecalPosition;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Start sampling stroke, takes mouse position as the start way point
	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void StartSample(FVector2D MousePos);

	// Finish sampling stroke, takes mouse position as the end way point
	UFUNCTION(BlueprintCallable, Category = "Drawing")
	FStroke FinishSample(FVector2D MousePos);

	/** This function should be called immediately after the whiteboard is spawned 
	  * (in other words, before player start to draw) 
	  * canvas size is passed in order to determine the sample distance
	  * @param Size size of the drawing canvas(in raw screen scale)
	  */
	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void OnWhiteboardSpawned(float Size);

private:

	/** Max number of waypoints on a stroke that lie horizontally across the canvas 
	* This is a way of determing the distance between waypoints, we do not use
	* distance directly because screen size may varies from platform
	*/
	const int32 MaxSamplesAcrossScreen = 60;

	UPROPERTY()
	FSymbol Symbol;

	FVector2D LastSamplePoint;

	float CanvasSize = -1;
	float WayPointSampleDistance = -1; // Distance between way points
	bool bIsSampling = false;

	void SampleMouse();

	/** Create a turing point between two given waypoints on the newest stroke
	 * @param PointIndex1 index of the start point 
	 * @param PointIndex2 index of the end point
	 * @param Priority priority of the feature point
	 * @return true if the turning point is created, false if not
	 */
	bool GenerateTurningPoint(int32 PointIndex1, int32 PointIndex2, int32 Priority);

	// Calculate the direction of feature point for current stroke, should be called after turning points are generated
	void CalculateFeaturePointDirection();

	bool MouseOutOfRange();
};
