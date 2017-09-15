// Fill out your copyright notice in the Description page of Project Settings.

#include "DataSingletonFunctionLibrary.h"
#include "Logogram/CharacterLibrary.h"

UCharacterLibrary * UDataSingletonFunctionLibrary::CharacterLibrary(NULL);

UCharacterLibrary * UDataSingletonFunctionLibrary::GetCharacterLibrary()
{
	if (!CharacterLibrary)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character library created"))
		CharacterLibrary = NewObject<UCharacterLibrary>();
		CharacterLibrary->ReadCharacter();
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character library exists"))
	}

	return CharacterLibrary;
}

void UDataSingletonFunctionLibrary::DestroyCharacterLibrary()
{
	CharacterLibrary = nullptr;
}
