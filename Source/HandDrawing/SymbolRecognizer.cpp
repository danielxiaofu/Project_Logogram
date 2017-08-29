// Fill out your copyright notice in the Description page of Project Settings.


#include "SymbolRecognizer.h"


// Sets default values for this component's properties
USymbolRecognizer::USymbolRecognizer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USymbolRecognizer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USymbolRecognizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USymbolRecognizer::ImportNewSymbol(USymbolCharacter * NewSymb)
{
	NewSymbol = NewSymb;
}

