// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BCIVNDKLevelAsset.generated.h"

USTRUCT()
struct FBCIVNDKAssetsLevelAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class UWorld> Level;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<class ULevelSequence> OpeningSequence;
	
};

/**
 * 
 */
UCLASS(MinimalAPI)
class UBCIVNDKAssetsLevelAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
	UPROPERTY(AssetRegistrySearchable, EditDefaultsOnly)
	FString SerializedLevelAssetData;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	FBCIVNDKAssetsLevelAssetData LevelAssetData;
	
};
