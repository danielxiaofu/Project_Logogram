// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterLibrary.h"

void UCharacterLibrary::ReadCharacter()
{
	if (bHasInitialized)
		return;

	// Read the json file that contains all symbol characters
	FString ReadDirectory = FPaths::GameContentDir();
	FString FileName = FString("Symbols.txt");
	FString TextToRead;

	FString AbsolutePath = ReadDirectory + "/" + FileName;
	FFileHelper::LoadFileToString(TextToRead, *AbsolutePath);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TextToRead);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> CharacterArray = JsonObject->GetArrayField(TEXT("symbol"));
		UE_LOG(LogTemp, Warning, TEXT("Character array retrieved"))
		for (TSharedPtr<FJsonValue> SymbolValue : CharacterArray) 
		{
			TSharedPtr<FJsonObject> SymbolObject;
			SymbolObject = SymbolValue->AsObject();
			FString Name = SymbolObject->GetStringField(TEXT("name"));
			int32 Type = SymbolObject->GetIntegerField(TEXT("type"));
			FLibraryCharacter& NewCharacter = AddNewCharacter(Name, Type);

			TArray<TSharedPtr<FJsonValue>> StrokeArray = SymbolObject->GetArrayField(TEXT("stroke"));
			for (TSharedPtr<FJsonValue> StrokeValue : StrokeArray)
			{
				TSharedPtr<FJsonObject> StrokeObject;
				StrokeObject = StrokeValue->AsObject();
				NewCharacter.AddNewStroke();

				TArray<TSharedPtr<FJsonValue>> PointArray = StrokeObject->GetArrayField(TEXT("point"));
				for (TSharedPtr<FJsonValue> PointValue : PointArray)
				{
					TSharedPtr<FJsonObject> PointObject;
					PointObject = PointValue->AsObject();
					float Direction = PointObject->GetNumberField(TEXT("dir"));
					NewCharacter.AddDirection(Direction);
				}
				
			}

			UE_LOG(LogTemp, Warning, TEXT("Character retrieved: %s"), *(NewCharacter.ToString()))
		}
		bHasInitialized = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("File not deserializable"))
		bHasInitialized = false;
	}

	
}

FLibraryCharacter& UCharacterLibrary::AddNewCharacter(FString CharName, int32 TypeInt)
{
	ECharacterType NewType = ECharacterType::VE_NotDefined;
	if (TypeInt == 1)
		NewType = ECharacterType::VE_Primitive;
	else if (TypeInt == 2)
		NewType = ECharacterType::VE_Container;
	else if (TypeInt == 3)
		NewType = ECharacterType::VE_LeftRadical;
	else if (TypeInt == 4)
		NewType = ECharacterType::VE_BottomRadical;
	else if (TypeInt == 5)
		NewType = ECharacterType::VE_RightRadical;

	LibraryCharacters.Add(FLibraryCharacter(CharName, NewType));

	return LibraryCharacters.Last();
}

bool UCharacterLibrary::HasInitialized()
{
	return bHasInitialized;
}

TArray<FLibraryCharacter>& UCharacterLibrary::RetrieveAllCharacter()
{
	TArray<FLibraryCharacter>& Characters = LibraryCharacters;

	return Characters;
}
