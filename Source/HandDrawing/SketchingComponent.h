// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logogram/SymbolCharacter.h"
#include "SketchingComponent.generated.h"

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVectorDelegate, const FVector&, WorldPosition);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointDirectionDelegate, float, NewFeaturePoint);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewStrokeDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStrokeFinishDelegate);

	USketchingComponent();

	// Fired when a waypoint is added to the current stroke
	UPROPERTY(BlueprintAssignable, Category = "Drawing")
	FVectorDelegate WaypointDelegate;

	// Fired when a feature point is created
	UPROPERTY()
	FPointDirectionDelegate PointDirectionDelegate;

	// Fired when a new stroke is created
	UPROPERTY()
	FNewStrokeDelegate NewStrokeDelegate;

	// Fired when current stroke is finished
	UPROPERTY()
	FStrokeFinishDelegate StrokeFinishDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableDebugMessage = false;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Start sampling stroke, takes brush position as start waypoint
	 * @param BrushPos world position of the brush
	 */
	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void StartSample(FVector WorldBrushPos);

	/** Finish sampling stroke, takes brush position as end wa point
	 * @param BrushPos world position of the brush
	 */
	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void FinishSample(FVector WorldBrushPos);

	/** Request to add a sample point, should only be called during sampling
	 * @param BrushPos world position of the brush
	 */
	void ReqestToAddSample(FVector WorldBrushPos);

	/** Set board axis in order to convert from screen position to world on-board pos
	* @param AxisX non-normalized X axis of the board 
	* @param AxisY non-normalized Y axis of the board
	*/
	void SetBoardRawAxis(FVector AxisX, FVector AxisY, FVector BottomLeftCorner, FVector TopRightCorner);

	// Debug only, get waypoints in the last stroke
	UFUNCTION(BlueprintCallable, Category = "Debug")
	TArray<FVector> GetLastWaypoints();

	// Debug only, get featurepoints in the last stroke
	UFUNCTION(BlueprintCallable, Category = "Debug")
	TArray<FVector> GetLastFeaturePoints();

	/** Clear symbol data, calls clean method in USymbolCharacter
	*/
	void ClearPendingSymbol();

private:

	/** Max number of waypoints on a stroke that lie horizontally across the canvas 
	 * This is a way of determing the distance between waypoints, we do not use
	 * distance directly because screen size may varies from platform
	 */
	const int32 MaxSamplesAcrossScreen = 60;

	UPROPERTY()
	USymbolCharacter* NewSymbol;

	UPROPERTY()
	APlayerController* PlayerController;

	FVector2D LastSamplePoint;

	FVector BoardRawAxisX = FVector(0);
	FVector BoardRawAxisY = FVector(0);
	FVector BottomLeft = FVector(0);
	FVector TopRight = FVector(0);

	float CanvasSize = -1;
	float WayPointSampleDistance = -1; // Distance between way points
	bool bIsSampling = false;

	/** Given board mesh, decide sample distance based on size of the board
	 * @param BoardMesh static mesh of the board
	 */
	void InitializeSampleDistance();

	/** Create a turing point between two given waypoints on the newest stroke
	 * @param PointIndex1 index of the start point 
	 * @param PointIndex2 index of the end point
	 * @param Priority priority of the feature point
	 * @return true if the turning point is created, false if not
	 */
	bool GenerateTurningPoint(int32 PointIndex1, int32 PointIndex2, int32 Priority);

	// Calculate the direction of feature point for current stroke, should be called after turning points are generated
	void CalculateFeaturePointDirection();

	/** Convert world position to board's local on-screen position with bottomleft being (0, 0)
	 * @param WorldPos world position
	 * @param LocalPos out parameter, converted local on-screen position 
	 */
	void ConvertWorldToLocalBoard(FVector WorldPos, FVector2D& LocalPos);

	/** Convert board's local on-screen position to world position
	 * @param LocalPos local position to be converted
	 * @param WorldPos out parameter, world position
	 */
	FVector ConvertLocalBoardToWorld(const FVector2D& LocalPos);
};
