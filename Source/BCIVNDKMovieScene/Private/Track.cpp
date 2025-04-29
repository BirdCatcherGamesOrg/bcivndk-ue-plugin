// SPDX-License-Identifier: MPL-2.0

#include "Track.h"

#include "ControlSection.h"
#include "EvalTemplate.h"
#include "Evaluation/MovieSceneEvaluationTrack.h"
#include "IMovieSceneTracksModule.h"
#include "BCIVNDKMovieScene/BCIVNDKMovieScene.h"
#include "Tracks/MovieSceneEventTrack.h"

#define LOCTEXT_NAMESPACE "BCIVNDKMovieSceneTrack"

UBCIVNDKMovieSceneTrack::UBCIVNDKMovieSceneTrack()
	: bFireEventsWhenForwards(true)
	, bFireEventsWhenBackwards(true)
	, EventPosition(EFireEventsAtPosition::AfterSpawn)
{
#if WITH_EDITOR
	TrackTint = FColor(91, 75, 57);
#endif
}

void UBCIVNDKMovieSceneTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}

bool UBCIVNDKMovieSceneTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass->IsChildOf(UBCIVNDKMovieSceneControlSection::StaticClass());
}

UMovieSceneSection* UBCIVNDKMovieSceneTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, "MovieSceneSection", RF_Transactional);
}

const TArray<UMovieSceneSection*>& UBCIVNDKMovieSceneTrack::GetAllSections() const
{
	return Sections;
}

bool UBCIVNDKMovieSceneTrack::HasSection(const UMovieSceneSection& Section) const
{
	return Sections.Contains(&Section);
}

bool UBCIVNDKMovieSceneTrack::IsEmpty() const
{
	return Sections.Num() == 0;
}

void UBCIVNDKMovieSceneTrack::RemoveAllAnimationData()
{
	Sections.Empty();
}

void UBCIVNDKMovieSceneTrack::RemoveSection(UMovieSceneSection& Section)
{
	Sections.Remove(&Section);
}

void UBCIVNDKMovieSceneTrack::RemoveSectionAt(int32 SectionIndex)
{
	Sections.RemoveAt(SectionIndex);
}

FMovieSceneEvalTemplatePtr UBCIVNDKMovieSceneTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	if (const UBCIVNDKMovieSceneControlSection* TriggerSection = Cast<const UBCIVNDKMovieSceneControlSection>(&InSection))
	{
		FBCIVNDKMovieSceneEvalTemplate Template;
		Template.AddData(TriggerSection->GetChannel().GetData());
		FBCIVNDKMovieSceneModule* Module = FModuleManager::GetModulePtr<FBCIVNDKMovieSceneModule>("BCIVNDKMovieScene");
		if (Module)
		{
			for (const UMovieSceneSection* Section : Sections)
			{
				FBCIVNDKMovieSceneModule::BridgeType* Bridge = Module->GetSceneSectionBridge(TSubclassOf<UMovieSceneSection>(Section->GetClass()));
				if (Bridge)
				{
					(*Bridge)(*Section, Template);
				}
			}
		}
		return Template;
	}
		
	return FMovieSceneEvalTemplatePtr();
}

void UBCIVNDKMovieSceneTrack::PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const
{
	switch (EventPosition)
	{
		case EFireEventsAtPosition::AtStartOfEvaluation:
			Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::PreEvaluation));
			break;

		case EFireEventsAtPosition::AtEndOfEvaluation:
			Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::PostEvaluation));
			break;

		default:
			Track.SetEvaluationGroup(IMovieSceneTracksModule::GetEvaluationGroupName(EBuiltInEvaluationGroup::SpawnObjects));
			Track.SetEvaluationPriority(UMovieSceneSpawnTrack::GetEvaluationPriority() - 100);
			break;
	}

	Track.SetEvaluationMethod(EEvaluationMethod::Swept);
}

FMovieSceneTrackSegmentBlenderPtr UBCIVNDKMovieSceneTrack::GetTrackSegmentBlender() const
{
	// This is a temporary measure to alleviate some issues with event tracks with finite ranges.
	// By filling empty space between sections, we're essentially always making this track evaluate
	// which allows it to sweep sections correctly when the play-head moves from a finite section
	// to empty space. This doesn't address the issue of the play-head moving from inside a sub-sequence
	// to outside, but that specific issue is even more nuanced and complicated to address.

	struct FMainSceneTrackSegmentBlender : FMovieSceneTrackSegmentBlender
	{
		FMainSceneTrackSegmentBlender()
		{
			bCanFillEmptySpace = true;
			bAllowEmptySegments = true;
		}

		virtual TOptional<FMovieSceneSegment> InsertEmptySpace(const TRange<FFrameNumber>& Range, const FMovieSceneSegment* PreviousSegment, const FMovieSceneSegment* NextSegment) const override
		{
			return FMovieSceneSegment(Range);
		}
	};

	return FMainSceneTrackSegmentBlender();
}

#if WITH_EDITORONLY_DATA

FText UBCIVNDKMovieSceneTrack::GetDefaultDisplayName() const
{
	return LOCTEXT("TrackName", "BCIVNDK Track");
}

#endif

#undef LOCTEXT_NAMESPACE
