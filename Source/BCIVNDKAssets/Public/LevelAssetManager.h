// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "BCIVNDKLevelAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelAssetManager.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UBCIVNDKAssetsLevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	BCIVNDKASSETS_API bool GetLevelAssetData(const FSoftObjectPath& Level, FBCIVNDKAssetsLevelAssetData& Data);

private:

	UPROPERTY(Transient)
	TMap<FSoftObjectPath, FBCIVNDKAssetsLevelAssetData> LevelAssets;
	
};
