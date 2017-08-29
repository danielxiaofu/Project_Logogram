// Fill out your copyright notice in the Description page of Project Settings.

#include "BrushConstrain.h"


// Sets default values for this component's properties
UBrushConstrain::UBrushConstrain()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrushConstrain::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBrushConstrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBrushConstrain::SetBoardAxis(FVector Horizontal, FVector Vertical)
{
	North = Vertical;
	East = Horizontal;
}
