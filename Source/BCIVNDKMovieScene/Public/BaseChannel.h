// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Curves/KeyHandle.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"

template
<typename ChannelType, typename ValueType>
struct TBCIVNDKMovieSceneBaseChannel : FMovieSceneChannel
{
	static const UScriptStruct& GetImpl() { return *ChannelType::StaticStruct(); };
	
	/**
	 * Common mutable interface to the time/value pairs
	 */
	FORCEINLINE TMovieSceneChannelData<ValueType> GetData()
	{
		return TMovieSceneChannelData<ValueType>(&static_cast<ChannelType*>(this)->Times, &static_cast<ChannelType*>(this)->KeyValues, this, &KeyHandles);
	}

	/**
	 * Common read-only interface to the time/value pairs
	 */
	FORCEINLINE TMovieSceneChannelData<const ValueType> GetData() const
	{
		return TMovieSceneChannelData<const ValueType>(&static_cast<const ChannelType*>(this)->Times, &static_cast<const ChannelType*>(this)->KeyValues);
	}
	
	/**
	 * Evaluate this curve at the specified time, returning whether a valid output value was assigned
	 *
	 * @param InTime        The time at which to evaluate
	 * @param OutValue      A value to receive the evaluated result, if possible
	 * @return True if the data was evaluated successfully, false otherwise
	 */
	bool Evaluate(FFrameTime InTime, ValueType& OutValue) const;

	/**
	 * Called after this section data has been serialized to upgrade old data from floating point times to integer ticks
	 */
	void PostSerialize(const FArchive& Ar);

	// ~ FMovieSceneChannel Interface
	virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Optimize(const FKeyDataOptimizationParams& InParameters) override;
	virtual void Offset(FFrameNumber DeltaPosition) override;
	virtual void Reset() override;

private:

	/** Transient key handles */
	FKeyHandleLookupTable KeyHandles;

#if WITH_EDITORONLY_DATA

	UPROPERTY()
	TArray<float> KeyTimes_DEPRECATED;

#endif
};

template <typename ChannelType, typename ValueType>
bool TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::Evaluate(FFrameTime InTime, ValueType& OutValue) const
{
	if (static_cast<const ChannelType*>(this)->Times.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(static_cast<const ChannelType*>(this)->Times, InTime.FrameNumber)-1);
		OutValue = static_cast<const ChannelType*>(this)->KeyValues[Index];
		return true;
	}

	return false;
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::PostSerialize(const FArchive& Ar)
{
#if WITH_EDITORONLY_DATA
if (KeyTimes_DEPRECATED.Num())
{
	FFrameRate LegacyFrameRate = GetLegacyConversionFrameRate();

	TArray<ValueType> OldValues = static_cast<ChannelType&>(*this).KeyValues;
	static_cast<ChannelType&>(*this).Times.Reset(KeyTimes_DEPRECATED.Num());
	static_cast<ChannelType&>(*this).KeyValues.Reset(KeyTimes_DEPRECATED.Num());
	for (int32 Index = 0; Index < KeyTimes_DEPRECATED.Num(); ++Index)
	{
		FFrameNumber KeyTime = UpgradeLegacyMovieSceneTime(nullptr, LegacyFrameRate, KeyTimes_DEPRECATED[Index]);
		ConvertInsertAndSort<ValueType>(Index, KeyTime, OldValues[Index], static_cast<ChannelType&>(*this).Times, static_cast<ChannelType&>(*this).KeyValues);
	}
	KeyTimes_DEPRECATED.Empty();
}
#endif
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

template <typename ChannelType, typename ValueType>
TRange<FFrameNumber> TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

template <typename ChannelType, typename ValueType>
int32 TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::GetNumKeys() const
{
	return static_cast<const ChannelType&>(*this).Times.Num();
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::Optimize(const FKeyDataOptimizationParams& InParameters)
{
	// In MovieSceneChannelTraits.h
	UE::MovieScene::Optimize(this, InParameters);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

template <typename ChannelType, typename ValueType>
void TBCIVNDKMovieSceneBaseChannel<ChannelType, ValueType>::Reset()
{
	static_cast<ChannelType&>(*this).Times.Reset();
	static_cast<ChannelType&>(*this).KeyValues.Reset();
	KeyHandles.Reset();
}