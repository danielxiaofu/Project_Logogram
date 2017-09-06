// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logogram/SymbolCharacter.h"
#include "Logogram/LibraryCharacterPrimitive.h"
#include "SymbolRecognizer.generated.h"

class USketchingComponent;

/** Stroke that contains only direction of feature points
*/
USTRUCT()
struct FFeaturedStroke {
	GENERATED_BODY()

	UPROPERTY()
	TArray<float> FeatureDirections;

	void AddFeatureDirections(float Fd)
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
};

/** This class takes SymbolCharacter created in SketchingComponent and compare it with
* every Character in CharacterLibrary
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

	/** Called when a SketchingComponent is created, the pointer is passed in 
	* in order to bind delegates
	* @param SketchingComponent pointer of the USketchingComponent
	*/
	void OnSketchingComponentCreated(USketchingComponent* SketchingComponent);

	UFUNCTION()
	void OnFeaturePointCreated(const FFeaturePoint& FeaturePoint);
};
