// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "BCIVNDKScriptSettings.generated.h"

UCLASS(MinimalAPI)
class UBCIVNDKScriptSettings : public UDeveloperSettings
{

	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	FPrimaryAssetType CharacterDataAssetType;
	
};
