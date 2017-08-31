// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrushMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANDDRAWING_API UBrushMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBrushPositionDelegate, const FVector&, WorldPosition);

	// Sets default values for this component's properties
	UBrushMovementComponent();

	// Fired when BrushPosition is updated
	UPROPERTY(BlueprintAssignable)
	FBrushPositionDelegate BrushPositionDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	float BrushSpeed = 500;

	UPROPERTY(BlueprintReadWrite)
	FVector2D BrushScreenVelocity;

	UPROPERTY(BlueprintReadWrite)
	FVector BrushPosition;
	
	/** Set board axis in order to convert between screen position to world on-board pos
	* @param AxisX X axis of the board
	* @param AxisY Y axis of the board
	* @param BottomLeft bottomleft corner of the board
	* @param TopRight topright corner of the board
	*/
	void SetBoardAxis(FVector AxisX, FVector AxisY, FVector BottomLeft, FVector TopRight);

private:

	FVector BoardAxisX = FVector(0);
	FVector BoardAxisY = FVector(0);
	FVector Origin = FVector(0);
	FVector TopRightOrigin = FVector(0);

	FVector2D North, South, East, West, NorthEast, NorthWest, SouthEast, SouthWest;

	void UpdateBrushPosition(float DeltaTime);

	/** Get the direction that the brush should be constrained to
	* @param RawDirection raw direction to be constrained
	* @return constrain direction
	*/
	FVector2D GetConstrainDirection(FVector2D RawDirection) const;

};
