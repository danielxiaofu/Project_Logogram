// Fill out your copyright notice in the Description page of Project Settings.

#include "DataSingleton.h"
#include "Logogram/CharacterLibrary.h"

UDataSingleton::UDataSingleton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

UCharacterLibrary * UDataSingleton::GetCharacterLibrary()
{
	if (!CharacterLibrary)
	{
		CharacterLibrary = NewObject<UCharacterLibrary>();
		CharacterLibrary->Initialize();
	}

	return CharacterLibrary;
}

