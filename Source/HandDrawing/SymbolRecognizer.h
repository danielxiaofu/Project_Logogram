// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logogram/SymbolCharacter.h"
#include "Logogram/LibraryCharacterPrimitive.h"
#include "SymbolRecognizer.generated.h"

class USketchingComponent;
class UCharacterLibrary;

/** Stroke that contains only direction of feature points
*/
USTRUCT()
struct FFeaturedStroke {
	GENERATED_BODY()

	UPROPERTY()
	TArray<float> FeatureDirections;

	void AddFeatureDirection(float Fd)
	{
		FeatureDirections.Add(Fd);
	}

	float GetFeatureDirection(int32 Index)
	{
		return FeatureDirections[Index];
	}

	TArray<float>& GetAllDirections()
	{
		return FeatureDirections;
	}

	FString ToString()
	{
		FString String;
		for (float F : FeatureDirections)
		{
			String += FString::SanitizeFloat(F) + ", ";
		}
		return String;
	}
};

USTRUCT(BlueprintType)
struct FCompareResult {
	GENERATED_BODY()

	UPROPERTY()
	ECharacterType CharacterType;

	UPROPERTY()
	float DirectionDiff;

	UPROPERTY()
	FString Name;

	FCompareResult() {
		CharacterType = ECharacterType::VE_NotDefined;
		DirectionDiff = 0;
		Name = FString("Unknown");
	}

	FString ToString() {
		FString String ="Compare result: Name: " + Name;
		String += " Difference: " + FString::SanitizeFloat(DirectionDiff);
		return String;
	}
};

/** This class takes stroke data that are created in SketchingComponent and convert them into
* type FeatureStroke, a struct that contains only the direction of feature points. Then the FeatureStroke
* will be compared with each character in the library and a CompareResult type will be generated 
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANDDRAWING_API USymbolRecognizer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USymbolRecognizer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// The symbol/character waiting to be recognized
	UPROPERTY()
	TArray<FFeaturedStroke> PendingSymbol;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableDebugMessage = false;

	/** Called when a SketchingComponent is created, the pointer is passed 
	* in order to bind delegates
	* @param SketchingComponent pointer of the USketchingComponent
	*/
	void OnSketchingComponentCreated(USketchingComponent* SketchingComponent);

	/** Called when a feature point has calculated its direction
	* @param Dir diraction of the feature point
	*/
	UFUNCTION()
	void OnPointDirectionComputed(float Dir);

	/** Called when a stroke is created
	*/
	UFUNCTION()
	void OnStrokeCreated();

	/** Called when current stroke is finished
	*/
	UFUNCTION()
	void OnStrokeFinished();

	void ClearPendingSymbol();

private:

	/** This variable determines how much does stroke number difference contribute to overall difference of a symbol
	*/
	const float PenaltyPerStrokeNum = PI;

	UPROPERTY()
	UCharacterLibrary* CharacterLibrary;

	/** Compare the pending symbol with all characters in the library
	* @return FCompareResult the result of comparision
	*/
	UFUNCTION()
	FCompareResult CompareAll();

	float CompareSymbolDifference(const TArray<FFeaturedStroke>& TargetSymbol, const FLibraryCharacter& LibrarySymbol);

	float CompareStrokeDifference(const FFeaturedStroke& TargetStroke, const FLibraryStroke& LibraryStroke);

	float ComparePointDifference(float TargetDirection, float LibraryDirection);
};
