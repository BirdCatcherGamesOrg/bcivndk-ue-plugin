// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "CharacterAssetData.generated.h"

UENUM()
enum class EBCIVNDKCharacterDataVersion : int32
{
	V1 = 1,
	LATEST = V1
};

USTRUCT()
struct FBCIVNDKCharacterAssetData
{

	GENERATED_BODY()

	UPROPERTY()
	EBCIVNDKCharacterDataVersion Version = EBCIVNDKCharacterDataVersion::LATEST;
	
	UPROPERTY()
	FString InternalName;
	
	UPROPERTY()
	FText DisplayName;

	UPROPERTY()
	FSlateWidgetStyle Styles;
	
};
