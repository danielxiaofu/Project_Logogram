// Fill out your copyright notice in the Description page of Project Settings.


#include "SymbolRecognizer.h"
#include "SketchingComponent.h"
#include "DataSingletonFunctionLibrary.h"

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
	CharacterLibrary = UDataSingletonFunctionLibrary::GetCharacterLibrary();
	
}


// Called every frame
void USymbolRecognizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USymbolRecognizer::OnSketchingComponentCreated(USketchingComponent * SketchingComponent)
{
	SketchingComponent->PointDirectionDelegate.AddDynamic(this, &USymbolRecognizer::OnPointDirectionComputed);
	SketchingComponent->NewStrokeDelegate.AddDynamic(this, &USymbolRecognizer::OnStrokeCreated);
	SketchingComponent->StrokeFinishDelegate.AddDynamic(this, &USymbolRecognizer::OnStrokeFinished);
}

void USymbolRecognizer::OnPointDirectionComputed(float Dir)
{
	if (PendingSymbol.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No feature stroke found in PendingSymbol, make sure it has at least one feature stroke before adding direction"))
		return;
	}
	
	// Add the direction to the newest feature stroke
	PendingSymbol.Last().AddFeatureDirection(Dir);
}

void USymbolRecognizer::OnStrokeCreated()
{
	// Add a new feature stroke
	PendingSymbol.Add(FFeaturedStroke());
}

void USymbolRecognizer::OnStrokeFinished()
{
	if (bEnableDebugMessage)
	{
		for (int32 i = 0; i < PendingSymbol.Num(); i++)
		{
			FString String = "Stroke " + FString::FromInt(i) + ": " + PendingSymbol[i].ToString();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *String)
		}
	}


}


