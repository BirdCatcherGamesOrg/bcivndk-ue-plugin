// SPDX-License-Identifier: MPL-2.0

#include "BCIVNDKLevelAsset.h"

#include "JsonObjectConverter.h"

void UBCIVNDKAssetsLevelAsset::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	FJsonObjectConverter::UStructToJsonObjectString(LevelAssetData, SerializedLevelAssetData);
}
