// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Whiteboard.generated.h"

class USketchingComponent;
class UBrushMovementComponent;
class USymbolRecognizer;

UENUM(BlueprintType)
enum class EDrawMode : uint8
{
	VE_Mouse UMETA(DisplayName = "Mouse"),
	VE_Keyboard UMETA(DisplayName = "Keyboard"),
	VE_Gamepad UMETA(DisplayName = "Gamepad"),
	VE_GamepadConstrained UMETA(DisplayName = "GamepadConstrained")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawMode")
	EDrawMode DrawMode = EDrawMode::VE_Mouse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USketchingComponent* SketchingComponent;

	UPROPERTY(BlueprintReadOnly)
	UBrushMovementComponent* BrushMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USymbolRecognizer* SymbolRecognizer;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartDraw();

	UFUNCTION(BlueprintCallable)
	void EndDraw();

	/** This function resets all drawing results. All pending symbols, strokes, points and compare results in all actor components will be cleared
	*/
	UFUNCTION(BlueprintCallable)
	void ResetWhiteBoard();

	UFUNCTION(BlueprintCallable)
	void ChangeDrawMode(EDrawMode DrawM);


private:

	UPROPERTY()
	UStaticMeshComponent* BoardMesh;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool bIsDrawing;

	FVector BoardAxisY, BoardAxisX;

};
