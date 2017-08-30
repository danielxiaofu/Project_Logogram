// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Whiteboard.generated.h"

class USketchingComponent;
class UBrushConstrain;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	USketchingComponent* SketchingComponent;

	UPROPERTY(BlueprintReadWrite)
	FVector2D BrushVelocity;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BrushDirectionUpdated(FVector BrushDirection);

private:
	UPROPERTY()
	UBrushConstrain* BrushConstrain;

	UPROPERTY()
	UStaticMeshComponent* BoardMesh;

	FVector BoardAxisY, BoardAxisX;

	

	void UpdateBrushDirection();

};
