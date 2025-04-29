// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "UserData.h"
#include "PresentationTableRow.generated.h"

UENUM()
enum class EBCIVNDKScriptPresentationTableRowVersion : int32
{
	V1 = 1,
	LATEST = V1
};

USTRUCT()
struct FBCIVNDKScriptPresentationTableRowData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetId Speaker;
	
	UPROPERTY(EditDefaultsOnly)
	FText PresentationText;

	UPROPERTY(EditDefaultsOnly)
	FBCICoreUserData UserData;
	
};

/**
 * 
 */
USTRUCT()
struct FBCIVNDKScriptPresentationTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FDataTableRowHandle SourceTableRow;

	UPROPERTY()
	FString CachedScriptLineHash;

	UPROPERTY()
	EBCIVNDKScriptPresentationTableRowVersion Version = EBCIVNDKScriptPresentationTableRowVersion::LATEST;

	UPROPERTY()
	FBCIVNDKScriptPresentationTableRowData Data;
	
	UPROPERTY()
	FString SerializedData;
	
};