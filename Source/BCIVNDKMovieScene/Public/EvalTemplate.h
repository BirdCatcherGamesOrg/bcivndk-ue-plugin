// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Broadcaster.h"
#include "JsonObjectConverter.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "EvalTemplate.generated.h"

USTRUCT()
struct FBCIVNDKMovieSceneEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
	
	template
	<typename ChannelType>
	void AddData(const TMovieSceneChannelData<const ChannelType>& Data);

private:

	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;

	UPROPERTY()
    TArray<FBCIVNDKControlBroadcast> Broadcasts;
	
	UPROPERTY()
	bool bFireEventsWhenForwards = true;

	UPROPERTY()
	bool bFireEventsWhenBackwards = true;
	
};

template <typename ChannelType>
void FBCIVNDKMovieSceneEvalTemplate::AddData(const TMovieSceneChannelData<const ChannelType>& Data)
{
	TArrayView<const FFrameNumber> Times = Data.GetTimes();
	const TArrayView<const ChannelType> Values = Data.GetValues();
	for (int i = 0; i < Times.Num(); i++)
	{
		FBCIVNDKControlBroadcast Broadcast;
		Broadcast.Time = Times[i];
		int32 TokenIndex = Broadcasts.AddUnique(Broadcast);
		TSharedPtr<FJsonObject> Object = FJsonObjectConverter::UStructToJsonObject(Values[i]);
		Broadcasts[TokenIndex].TokenValues.JsonObject->SetField(ChannelType::StaticStruct()->GetName(), MakeShared<FJsonValueObject>(Object));
	}
	Broadcasts.Sort();
}

template
<typename ChannelType, typename ValueType>
struct TBCIVNKDKMovieSceneBaseEvalTemplateBridge
{
	static void AddData(const UMovieSceneSection& CustomSection, FBCIVNDKMovieSceneEvalTemplate& Template);
};

template
<typename ChannelType, typename ValueType>
void TBCIVNKDKMovieSceneBaseEvalTemplateBridge<ChannelType, ValueType>::AddData(const UMovieSceneSection& Section, FBCIVNDKMovieSceneEvalTemplate& Template)
{
	FMovieSceneChannelProxy& Proxy = Section.GetChannelProxy();
	TArrayView<ChannelType*> Channels = Proxy.GetChannels<ChannelType>();
	for (const ChannelType* Channel : Channels)
	{
		if (!Channel)
		{
			continue;
		}
		const TMovieSceneChannelData<const ValueType> Data = Channel->GetData();
		Template.AddData<ValueType>(Data);
	}
}

template
<typename ChannelType, typename ValueType>
struct TBCIVNDKMovieSceneSectionTemplateBridge : TBCIVNKDKMovieSceneBaseEvalTemplateBridge<ChannelType, ValueType>
{
};
