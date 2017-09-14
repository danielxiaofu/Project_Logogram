// Fill out your copyright notice in the Description page of Project Settings.


#include "SymbolRecognizer.h"
#include "SketchingComponent.h"
#include "DataSingletonFunctionLibrary.h"
#include "Logogram/CharacterLibrary.h"

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

void USymbolRecognizer::ClearPendingSymbol()
{
	PendingSymbol.Empty();
}

FCompareResult USymbolRecognizer::CompareAll()
{
	TArray<FLibraryCharacter>& AllChars = CharacterLibrary->RetrieveAllCharacter();
	for (int32 i = 0; i < AllChars.Num(); i++)
	{
		FLibraryCharacter& Char = AllChars[i];
	}

	return FCompareResult();
}

float USymbolRecognizer::CompareSymbolDifference(const TArray<FFeaturedStroke>& InputSymbol, const FLibraryCharacter& LibrarySymbol)
{
	//float Result;

	// Compute the difference of number of strokes
	float StrokeNumDiff = FMath::Abs(InputSymbol.Num() - LibrarySymbol.Strokes.Num());

	// Find the smaller number of strokes between two symbols
	int32 MinStrokeNum = FMath::Min(InputSymbol.Num(), LibrarySymbol.Strokes.Num());

	// Iterate through each stroke based on MinStrokeNum
	for (int32 i = 0; i < MinStrokeNum; i++)
	{
		const FFeaturedStroke& InputStroke = InputSymbol[i];
		const FLibraryStroke& LibraryStroke = LibrarySymbol.Strokes[i];

	}

	return 0.0f;
}

float USymbolRecognizer::CompareStrokeDifference(const FFeaturedStroke & TargetStroke, const FLibraryStroke & LibraryStroke)
{
	float Result = 0;

	// Find the smaller number of feature points between two strokes
	int32 MinPointNum = FMath::Min(TargetStroke.FeatureDirections.Num(), LibraryStroke.Directions.Num());

	// Iterate through each point based on MinPointNum
	for (int32 i = 0; i < MinPointNum; i++)
	{
		float TargetDirection = TargetStroke.FeatureDirections[i];
		float LibraryDirection = LibraryStroke.Directions[i];
		// Compare point difference
		Result += ComparePointDifference(TargetDirection, LibraryDirection);
	}

	return Result;
}

float USymbolRecognizer::ComparePointDifference(float TargetDirection, float LibraryDirection)
{
	// Since directions are in radians between 0 and 2 * PI, difference is determined by the minimum angle between two directions
	float Diff1 = FMath::Abs(LibraryDirection - TargetDirection);
	float Diff2 = FMath::Abs(2 * PI - Diff1);

	return Diff1 < Diff2 ? Diff1 : Diff2;
}


