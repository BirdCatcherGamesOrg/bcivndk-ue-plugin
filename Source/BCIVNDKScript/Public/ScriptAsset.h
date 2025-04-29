// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "ScriptAsset.generated.h"

UENUM()
enum class EBCIVNDKScriptAssetDataVersion : int32
{
	V1 = 1,
	LATEST = V1
};

USTRUCT()
struct FBCIVNDKScriptAssetData
{
	GENERATED_BODY()

	UPROPERTY()
	EBCIVNDKScriptAssetDataVersion Version = EBCIVNDKScriptAssetDataVersion::LATEST;

	UPROPERTY()
	TObjectPtr<class UDataTable> SourceScript;

	UPROPERTY()
	TObjectPtr<class UDataTable> PresentationScript;

	UPROPERTY()
	FBCICoreUserData UserData;
	
};
