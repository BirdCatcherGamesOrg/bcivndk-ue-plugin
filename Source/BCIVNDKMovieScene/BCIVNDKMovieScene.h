// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBCIVNDKMovieSceneModule : public IModuleInterface
{
public:
	using BridgeType = TFunction<void (const class UMovieSceneSection& Section, struct FBCIVNDKMovieSceneEvalTemplate& Template)>;

	template
	<typename SectionType>
	void AddSceneSectionBridge(BridgeType Bridge);

	template
	<typename SectionType>
	BridgeType* GetSceneSectionBridge();

	template
	<typename SectionType>
	BridgeType* GetSceneSectionBridge(const TSubclassOf<SectionType> Type);
	
private:
	
	TMap<FName, BridgeType> Bridges;

};

template <typename SectionType>
void FBCIVNDKMovieSceneModule::AddSceneSectionBridge(BridgeType Bridge)
{
	Bridges.Add(TSubclassOf<SectionType>()->GetFName(), Bridge);
}

template <typename SectionType>
FBCIVNDKMovieSceneModule::BridgeType* FBCIVNDKMovieSceneModule::GetSceneSectionBridge()
{
	return Bridges.Find(TSubclassOf<SectionType>()->GetFName());
}

template <typename SectionType>
FBCIVNDKMovieSceneModule::BridgeType* FBCIVNDKMovieSceneModule::GetSceneSectionBridge(const TSubclassOf<SectionType> Type)
{
	return Bridges.Find(Type->GetFName());
}
