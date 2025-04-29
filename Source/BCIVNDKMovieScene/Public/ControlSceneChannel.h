// SPDX-License-Identifier: MPL-2.0

#pragma once
#pragma warning(disable: 4584)

#include "BaseChannel.h"
#include "Payload.h"
#include "CoreTypes.h"
#include "UObject/ObjectMacros.h"
#include "Curves/KeyHandle.h"
#include "MovieSceneClipboard.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "ControlSceneChannel.generated.h"

/**
 * A Control channel of curve data that returns FControlChannelType values
 */
USTRUCT()
struct FBCIVNDKMovieSceneControlChannel
#if CPP
	: public TBCIVNDKMovieSceneBaseChannel<FBCIVNDKMovieSceneControlChannel, FBCIVNDKControlPayload>
#endif // CPP
{
	using Super = FMovieSceneChannel;
		
	GENERATED_BODY()
	
	/** Sorted array of key times as frame numbers in the sequence's tick resolution */
	UPROPERTY(meta=(KeyTimes))
	TArray<FFrameNumber> Times;

	/** Array of values that correspond to each key time */
	UPROPERTY(meta=(KeyValues))
	TArray<FBCIVNDKControlPayload> KeyValues;
	
};

/**
 * Movie Scene traits class defining several properties of TBCIMovieSceneChannel.
 * This specialization is only necessary if you want to change the default traits.
 */
template<>
struct TMovieSceneChannelTraits<FBCIVNDKMovieSceneControlChannel> : TMovieSceneChannelTraitsBase<FBCIVNDKMovieSceneControlChannel>
{
	/* This Control channel does not have default values */
	enum { SupportsDefaults = false };

#if WITH_EDITOR

	/* This Control channel supports retrieving an external value from an object */
	typedef TMovieSceneExternalValue<FBCIVNDKControlPayload> ExtendedEditorDataType;

#endif
};


namespace MovieSceneClipboard
{
	/** Specialization required for clipboard interaction for FMyControlChannelType keys */
	template<> inline FName GetKeyTypeName<FBCIVNDKControlPayload>()
	{
		return "BCIVNDKControlPayload";
	}
}