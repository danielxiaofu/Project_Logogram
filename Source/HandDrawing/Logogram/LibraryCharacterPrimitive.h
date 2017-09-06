// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LibraryCharacterPrimitive.generated.h"

UENUM()
enum class ECharacterType : uint8
{
	VE_NotDefined,
	VE_Container,
	VE_Primitive,
	VE_LeftRadical,
	VE_RightRadical,
	VE_BottomRadical

};

USTRUCT()
struct FLibraryStroke {
	GENERATED_BODY()

		UPROPERTY()
		TArray<float> Directions;

	void AddDirection(float Dir) {
		Directions.Add(Dir);
	}
};

USTRUCT()
struct FLibraryCharacter {

	GENERATED_BODY()

		UPROPERTY()
		TArray<FLibraryStroke> Strokes;

	UPROPERTY()
		FString Name;

	UPROPERTY()
		ECharacterType Type;

	FLibraryCharacter() {
		Type = ECharacterType::VE_NotDefined;
	}

	FLibraryCharacter(FString CharName, ECharacterType CharType) {
		Name = CharName;
		Type = CharType;
	}

	void AddNewStroke()
	{
		Strokes.Add(FLibraryStroke());
	}

	// Add a direction to the last stroke
	void AddDirection(float Direction)
	{
		Strokes.Last().AddDirection(Direction);
	}

	FString ToString()
	{
		FString String = Name + " :";
		for (int32 i = 0; i < Strokes.Num(); i++)
		{
			String += " Stroke " + FString::FromInt(i) + ":";
			for (float Dir : Strokes[i].Directions)
			{
				String += " " + FString::SanitizeFloat(Dir);
			}
			String += "; ";
		}
		return String;
	}
};

/**
 * 
 */
UCLASS()
class HANDDRAWING_API ULibraryCharacterPrimitive : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
