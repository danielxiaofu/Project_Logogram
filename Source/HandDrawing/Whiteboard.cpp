// Fill out your copyright notice in the Description page of Project Settings.

#include "Whiteboard.h"
#include "SketchingComponent.h"
#include "BrushConstrain.h"

// Sets default values
AWhiteboard::AWhiteboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SketchingComponent = CreateDefaultSubobject<USketchingComponent>(TEXT("SketchingComponent"));
	BrushConstrain = CreateDefaultSubobject<UBrushConstrain>(TEXT("BrushConstrain"));
}

// Called when the game starts or when spawned
void AWhiteboard::BeginPlay()
{
	Super::BeginPlay();
	
	/* Construct world on-board coordinate system
	*/
	BoardMesh = FindComponentByClass<UStaticMeshComponent>();
	FVector BottomLeft = BoardMesh->GetSocketLocation(FName("BottomLeft"));
	FVector TopLeft = BoardMesh->GetSocketLocation(FName("TopLeft"));
	FVector BottomRight = BoardMesh->GetSocketLocation(FName("BottomRight"));
	BoardAxisY = (TopLeft - BottomLeft).GetSafeNormal();
	BoardAxisX = (BottomRight - BottomLeft).GetSafeNormal();

}

// Called every frame
void AWhiteboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBrushDirection();
}

void AWhiteboard::StartDraw()
{
	bIsDrawing = true;
}

void AWhiteboard::EndDraw()
{
	bIsDrawing = false;
}

void AWhiteboard::UpdateBrushDirection()
{
	if (BrushVelocity.SizeSquared() == 0)
		return;

	FVector2D Direction = BrushConstrain->GetConstrainDirection(BrushVelocity.GetSafeNormal());
	FVector WorldDirection = BoardAxisX * Direction.X + BoardAxisY * Direction.Y;

	FVector FinalDirection = BrushVelocity.SizeSquared() > 1 ? WorldDirection.GetSafeNormal() : WorldDirection.GetSafeNormal() * BrushVelocity.Size();
	BrushDirectionUpdated(FinalDirection);
}

