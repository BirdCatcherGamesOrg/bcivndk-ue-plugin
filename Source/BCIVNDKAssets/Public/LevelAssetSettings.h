// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LevelAssetSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class UBCIVNDKAssetsLevelSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UBCIVNDKAssetsLevelSettings();
	
	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FPrimaryAssetType> LevelAssetClasses;
	
};
