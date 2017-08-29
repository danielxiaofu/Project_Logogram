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

	FVector North, South, East, West, NorthEast, NorthWest, SouthEast, SouthWest;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Set the board axis in order to get eight local directions
	* @param Horizontal normalized horizontal axis
	* @param Vertical normalized vertical axis
	*/
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetBoardAxis(FVector Horizontal, FVector Vertical);
	

};
