// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BrushConstrain.generated.h"

/* This component is responsible for constraining brush into 8 directions
*
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANDDRAWING_API UBrushConstrain : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBrushConstrain();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector2D North, South, East, West, NorthEast, NorthWest, SouthEast, SouthWest;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Get the direction that the brush should be constrained to
	* @param RawDirection raw direction to be constrained
	* @return constrain direction
	*/
	UFUNCTION(BlueprintCallable)
	FVector2D GetConstrainDirection(FVector2D RawDirection) const;
};
