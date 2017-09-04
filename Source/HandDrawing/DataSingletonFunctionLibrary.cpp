// Fill out your copyright notice in the Description page of Project Settings.

#include "DataSingletonFunctionLibrary.h"
#include "Logogram/CharacterLibrary.h"

UCharacterLibrary * UDataSingletonFunctionLibrary::CharacterLibrary(NULL);

UCharacterLibrary * UDataSingletonFunctionLibrary::GetCharacterLibrary()
{
	if (!CharacterLibrary)
	{
		CharacterLibrary = NewObject<UCharacterLibrary>();
	}

	return nullptr;
}
