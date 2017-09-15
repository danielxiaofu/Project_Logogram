// Fill out your copyright notice in the Description page of Project Settings.

#include "Whiteboard.h"
#include "SketchingComponent.h"
#include "BrushMovementComponent.h"
#include "SymbolRecognizer.h"

// Sets default values
AWhiteboard::AWhiteboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SketchingComponent = CreateDefaultSubobject<USketchingComponent>(TEXT("SketchingComponent"));
	BrushMovementComponent = CreateDefaultSubobject<UBrushMovementComponent>(TEXT("BrushMovement"));
	SymbolRecognizer = CreateDefaultSubobject<USymbolRecognizer>(TEXT("SymbolRecognizer"));

	SymbolRecognizer->OnSketchingComponentCreated(SketchingComponent);
}

// Called when the game starts or when spawned
void AWhiteboard::BeginPlay()
{
	Super::BeginPlay();
	
	ChangeDrawMode(DrawMode);

	/* Construct world on-board coordinate system
	*/
	BoardMesh = FindComponentByClass<UStaticMeshComponent>();
	FVector BottomLeft = BoardMesh->GetSocketLocation(FName("BottomLeft"));
	FVector TopLeft = BoardMesh->GetSocketLocation(FName("TopLeft"));
	FVector BottomRight = BoardMesh->GetSocketLocation(FName("BottomRight"));
	FVector TopRight = BoardMesh->GetSocketLocation(FName("TopRight"));
	BoardAxisY = (TopLeft - BottomLeft).GetSafeNormal();
	BoardAxisX = (BottomRight - BottomLeft).GetSafeNormal();
	// Pass board axises to child components
	SketchingComponent->SetBoardRawAxis(BottomRight - BottomLeft, TopLeft - BottomLeft, BottomLeft, TopRight);
	BrushMovementComponent->SetBoardAxis(BoardAxisX, BoardAxisY, BottomLeft, TopRight, TopLeft);
}

// Called every frame
void AWhiteboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDrawing)
		SketchingComponent->ReqestToAddSample(BrushMovementComponent->BrushPosition);
}

void AWhiteboard::StartDraw()
{
	bIsDrawing = true;
	SketchingComponent->StartSample(BrushMovementComponent->BrushPosition);
}

void AWhiteboard::EndDraw()
{
	SketchingComponent->FinishSample(BrushMovementComponent->BrushPosition);
	bIsDrawing = false;

}

void AWhiteboard::ResetWhiteBoard()
{
	if (bIsDrawing)
		return;

	SketchingComponent->ClearPendingSymbol();
	SymbolRecognizer->ClearPendingSymbol();
}

void AWhiteboard::ChangeDrawMode(EDrawMode DrawM)
{
	if (!BrushMovementComponent)
		return;

	DrawMode = DrawM;

	switch (DrawMode)
	{
	case EDrawMode::VE_Mouse:
		BrushMovementComponent->SetDirectMouseControl(true);
		break;
	case EDrawMode::VE_Keyboard:
		BrushMovementComponent->SetDirectMouseControl(false);
		break;
	case EDrawMode::VE_Gamepad:
		BrushMovementComponent->SetDirectMouseControl(false);
		BrushMovementComponent->SetEightDirectionMode(false);
		break;
	case EDrawMode::VE_GamepadConstrained:
		BrushMovementComponent->SetDirectMouseControl(false);
		BrushMovementComponent->SetEightDirectionMode(true);
		break;
	default:
		break;
	}
}





