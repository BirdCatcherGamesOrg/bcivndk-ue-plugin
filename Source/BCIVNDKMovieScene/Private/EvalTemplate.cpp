// SPDX-License-Identifier: MPL-2.0

#include "EvalTemplate.h"

#include "Broadcaster.h"
#include "ControlSection.h"
#include "MovieSceneSequencePlayer.h"
#include "IMovieScenePlayer.h"
#include "Evaluation/MovieSceneEvaluation.h"

struct FBCIVNDKMovieSceneExecutionToken final : IMovieSceneExecutionToken
{
	FBCIVNDKMovieSceneExecutionToken(TArray<int32>& InTokenIndexes, TArrayView<const FBCIVNDKControlBroadcast> InBroadcasts)
		: TokenIndexes(MoveTemp(InTokenIndexes)), Broadcasts(InBroadcasts)
	{
	}
	
	TArray<int32> TokenIndexes;

	TArrayView<const FBCIVNDKControlBroadcast> Broadcasts;

	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		MOVIESCENE_DETAILED_SCOPE_CYCLE_COUNTER(MovieSceneEval_DialogueLinesTrack_TokenExecute)

		if (!Player.AsUObject())
		{
			return;
		}
		
		AActor* LevelSequenceActor = Player.AsUObject()->GetTypedOuter<AActor>();

		if (!LevelSequenceActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to get actor"));
			return;
		}
		
		UBCIVNDKControlBroadcaster* Handler = LevelSequenceActor->FindComponentByClass<UBCIVNDKControlBroadcaster>();
		if (!Handler)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to get handler"));
			return;
		}
		
		for (int32 TokenIndex : TokenIndexes)
		{
			Handler->Handle(Broadcasts[TokenIndex]);
		}
	}
}; 

/*
FBCIVNDKMovieSceneEvalTemplate::FBCIVNDKMovieSceneEvalTemplate(const UBCIVNDKMovieSceneControlSection& Section)
{
	const TMovieSceneChannelData<const FBCIVNDKControlPayload> ControlData = Section.ControlPayload.GetData();
	AddData<const FBCIVNDKControlPayload>(ControlData);
}
*/

void FBCIVNDKMovieSceneEvalTemplate::EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// Don't allow events to fire when playback is in a stopped state. This can occur when stopping 
	// playback and returning the current position to the start of playback. It's not desirable to have 
	// all the events from the last playback position to the start of playback be fired.
	if (Context.GetStatus() == EMovieScenePlayerStatus::Stopped || Context.IsSilent())
	{
		return;
	}

	const bool bBackwards = Context.GetDirection() == EPlayDirection::Backwards;

	if ((!bBackwards && !bFireEventsWhenForwards) || (bBackwards && !bFireEventsWhenBackwards))
	{
		return;
	}

	TArray<int32> KeyIndexes;
	
	if (bBackwards)
	{
		// Trigger events backwards
		for (int32 KeyIndex = Broadcasts.Num() - 1; KeyIndex >= 0; --KeyIndex)
		{
			if (SweptRange.Contains(Broadcasts[KeyIndex].Time))
			{
				KeyIndexes.Add(KeyIndex);
			}
		}
	}
	else
	{
		// Trigger events forwards
		for (int32 KeyIndex = 0; KeyIndex < Broadcasts.Num(); ++KeyIndex)
		{
			if (SweptRange.Contains(Broadcasts[KeyIndex].Time))
			{
				KeyIndexes.Add(KeyIndex);
			}
		}
	}

	if (KeyIndexes.Num())
	{
		ExecutionTokens.Add(FBCIVNDKMovieSceneExecutionToken(KeyIndexes, MakeArrayView(Broadcasts)));
	}
}
