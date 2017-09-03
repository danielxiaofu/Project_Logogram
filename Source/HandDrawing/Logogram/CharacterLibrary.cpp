// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterLibrary.h"

void UCharacterLibrary::Initialize()
{
	// Read json file containing all symbol characters
	FString ReadDirectory = FPaths::GameContentDir();
	FString FileName = FString("Symbols.txt");
	FString TextToRead;

	FString AbsolutePath = ReadDirectory + "/" + FileName;
	FFileHelper::LoadFileToString(TextToRead, *AbsolutePath);

	UE_LOG(LogTemp, Warning, TEXT("Symbol file read"))

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextToRead);

	UE_LOG(LogTemp, Warning, TEXT("Json deserialization start"))

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		
		FString Name = JsonObject->GetStringField(TEXT("name"));
		UE_LOG(LogTemp, Warning, TEXT("Json name read: %s"), *Name)
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Json deserialization failed"))

	UE_LOG(LogTemp, Warning, TEXT("Json deserialization finish"))

	bHasInitialized = true;
}
