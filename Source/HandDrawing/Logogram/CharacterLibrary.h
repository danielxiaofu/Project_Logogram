// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LibraryCharacterPrimitive.h"
#include "CharacterLibrary.generated.h"


/** 
 * 
 */
UCLASS(BlueprintType)
class HANDDRAWING_API UCharacterLibrary : public UObject
{
	GENERATED_BODY()
	
private:
	bool bHasInitialized = false;

	UPROPERTY()
	TArray<FLibraryCharacter> LibraryCharacters;

	FLibraryCharacter& AddNewCharacter(FString CharName, int32 TypeInt);

public:
	void ReadCharacter();
	
	bool HasInitialized();

	TArray<FLibraryCharacter>& RetrieveAllCharacter();
};
