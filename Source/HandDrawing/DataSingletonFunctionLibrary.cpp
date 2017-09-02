// Fill out your copyright notice in the Description page of Project Settings.

#include "DataSingletonFunctionLibrary.h"

UDataSingleton* UDataSingletonFunctionLibrary::GetDataSingleton(bool& IsValid)
{
	IsValid = false;
	UDataSingleton* DataInstance = Cast<UDataSingleton>(GEngine->GameSingleton);
	if (!DataInstance) return nullptr;
	if (!DataInstance->IsValidLowLevel()) return nullptr;

	IsValid = true;
	return DataInstance;

}


