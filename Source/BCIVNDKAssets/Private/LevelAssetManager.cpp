// SPDX-License-Identifier: MPL-2.0

#include "LevelAssetManager.h"

#include "AssetDataUtilities.h"
#include "BCIVNDKAssetsLogCategory.h"
#include "BCIVNDKLevelAsset.h"
#include "LevelAssetSettings.h"
#include "Macros/SubsystemInitialization.h"

void UBCIVNDKAssetsLevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	BCILOG_INITIALIZE_SUBSYSTEM(LogBCIVNDKAssets);
	Super::Initialize(Collection);
	const UBCIVNDKAssetsLevelSettings* Settings = GetDefault<UBCIVNDKAssetsLevelSettings>();
	TArray<FBCIVNDKAssetsLevelAssetData> LevelAssetData;
	GET_ALL_JSON_ASSETS_CHECKED(Settings->LevelAssetClasses, UBCIVNDKAssetsLevelAsset, SerializedLevelAssetData, LevelAssetData);
	for (const FBCIVNDKAssetsLevelAssetData& Data : LevelAssetData)
	{
		LevelAssets.Add(Data.Level.ToSoftObjectPath(), Data);
	}
}

bool UBCIVNDKAssetsLevelManager::GetLevelAssetData(const FSoftObjectPath& Level, FBCIVNDKAssetsLevelAssetData& Data)
{
	FBCIVNDKAssetsLevelAssetData* LevelAssetData = LevelAssets.Find(Level);
	if (LevelAssetData == nullptr)
	{
		return false;
	}
	Data = *LevelAssetData;
	return true;
}
