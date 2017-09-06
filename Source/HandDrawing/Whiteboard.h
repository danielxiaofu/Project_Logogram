// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Whiteboard.generated.h"

class USketchingComponent;
class UBrushMovementComponent;
class USymbolRecognizer;

UCLASS()
class HANDDRAWING_API AWhiteboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWhiteboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly)
	USketchingComponent* SketchingComponent;

	UPROPERTY(BlueprintReadOnly)
	UBrushMovementComponent* BrushMovementComponent;

	UPROPERTY(BlueprintReadOnly)
	USymbolRecognizer* SymbolRecognizer;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartDraw();

	UFUNCTION(BlueprintCallable)
	void EndDraw();

private:

	UPROPERTY()
	UStaticMeshComponent* BoardMesh;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsDrawing;

	FVector BoardAxisY, BoardAxisX;

};
