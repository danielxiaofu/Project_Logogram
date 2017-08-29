// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logogram/SymbolCharacter.h"
#include "SymbolRecognizer.generated.h"


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

	UPROPERTY()
	USymbolCharacter* NewSymbol;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "SymbolRecognition")
	void ImportNewSymbol(USymbolCharacter* NewSymb);
	
};
