// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataSingleton.h"
#include "DataSingletonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HANDDRAWING_API UDataSingletonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure, Category = "Data Singleton")
	static UDataSingleton* GetDataSingleton(bool& IsValid);
	
	
	
};
