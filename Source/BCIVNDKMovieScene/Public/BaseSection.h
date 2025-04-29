// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Channels/MovieSceneChannelProxy.h"

template
<typename SectionType, typename ChannelType, typename ValueType>
class TBCIVNDKMovieSceneBaseSection
{
public:
	
	TBCIVNDKMovieSceneBaseSection();

	const ChannelType& GetChannel() const;
	
};

template
<typename SectionType, typename ChannelType, typename ValueType>
TBCIVNDKMovieSceneBaseSection<SectionType, ChannelType, ValueType>::TBCIVNDKMovieSceneBaseSection()
{
	static_cast<SectionType&>(*this).bSupportsInfiniteRange = true;
	static_cast<SectionType&>(*this).SetRange(TRange<FFrameNumber>::All());
	
	FMovieSceneChannelProxyData AllChannels;
	FMovieSceneChannelMetaData MetaData = FMovieSceneChannelMetaData("Channel", FText::FromString("Channel"));
	MetaData.bCanCollapseToTrack = false;
	AllChannels.Add(static_cast<SectionType&>(*this).Channel, MetaData, TMovieSceneExternalValue<ValueType>::Make());
	
	static_cast<SectionType&>(*this).ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(AllChannels));
}

template
<typename SectionType, typename ChannelType, typename ValueType>
const ChannelType& TBCIVNDKMovieSceneBaseSection<SectionType, ChannelType, ValueType>::GetChannel() const
{
	return static_cast<const SectionType&>(*this).Channel;
}
