// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseCharacter.generated.h"

/** Struct for a single stroke represented by array of directions of feature points
*/
USTRUCT(BlueprintType)
struct FStrokeDiractions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stroke")
	TArray<float> Directions;

	FStrokeDiractions()
	{

	}
};

/** Base class for all symbol characters in library
 * 
 */
UCLASS()
class HANDDRAWING_API UBaseCharacter : public UObject
{
	GENERATED_BODY()
	
public:

	/** Array of strokes 
	*/
	UPROPERTY(EditAnywhere)
	TArray<FStrokeDiractions> Strokes;
	
	UPROPERTY(EditAnywhere)
	FName CharacterName;
	
};
