// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SketchingComponent.generated.h"

class USymbolCharacter;

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
	void FinishSample(FVector2D MousePos);

	// Debug only, get waypoints in the last stroke
	UFUNCTION(BlueprintCallable, Category = "Debug")
	TArray<FVector2D> GetLastWaypoints();

	// Debug only, get featurepoints in the last stroke
	UFUNCTION(BlueprintCallable, Category = "Debug")
	TArray<FVector2D> GetLastFeaturePoints();

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
	USymbolCharacter* NewSymbol;

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
