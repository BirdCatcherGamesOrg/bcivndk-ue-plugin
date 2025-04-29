// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "BaseSection.h"
#include "ControlSceneChannel.h"
#include "MovieSceneSection.h"
#include "ControlSection.generated.h"

UCLASS(MinimalAPI)
class UBCIVNDKMovieSceneControlSection : public UMovieSceneSection, public TBCIVNDKMovieSceneBaseSection<UBCIVNDKMovieSceneControlSection, FBCIVNDKMovieSceneControlChannel, FBCIVNDKControlPayload>
{
	friend TBCIVNDKMovieSceneBaseSection;
	
	GENERATED_BODY()

	UPROPERTY()
	FBCIVNDKMovieSceneControlChannel Channel;
	
};
