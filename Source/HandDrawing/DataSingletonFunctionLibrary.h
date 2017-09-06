// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataSingletonFunctionLibrary.generated.h"

class UCharacterLibrary;
/**
 * 
 */
UCLASS()
class HANDDRAWING_API UDataSingletonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	static UCharacterLibrary* CharacterLibrary;

public:

	UFUNCTION(BlueprintCallable, Category = "CharacterLibrary")
	static UCharacterLibrary* GetCharacterLibrary();
	
	
};
