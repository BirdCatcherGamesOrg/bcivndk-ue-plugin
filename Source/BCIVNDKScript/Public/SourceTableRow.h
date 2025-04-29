// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "SourceTableRow.generated.h"

UENUM()
enum class EBCIVNDKScriptSourceTableRowVersion : int32
{
	V1 = 1,
	LATEST = V1
};

/**
 * 
 */
USTRUCT()
struct FBCIVNDKScriptSourceTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY()
	EBCIVNDKScriptSourceTableRowVersion Version = EBCIVNDKScriptSourceTableRowVersion::LATEST;
	
	UPROPERTY()
	FString SceneHeader;

	UPROPERTY()
	int32 SceneNumber = 0;

	UPROPERTY()
	FString SceneTags;
	
	UPROPERTY()
	FString SceneSection;

	UPROPERTY()
	int32 LineNumber = 0;
	
	UPROPERTY()
	FString Speaker;

	UPROPERTY()
	FString Line;

	UPROPERTY()
	FString Transition;
	
	UPROPERTY()
	FString LineTags;

	UPROPERTY()
	FBCICoreUserData UserData;

};