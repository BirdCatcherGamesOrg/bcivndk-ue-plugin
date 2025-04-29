// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneNameableTrack.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"
#include "Tracks/MovieSceneEventTrack.h"
#include "Track.generated.h"

UCLASS(MinimalAPI)
class UBCIVNDKMovieSceneTrack
	: public UMovieSceneNameableTrack
	, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()

	friend class FBCIVNDKSequencerTrackEditor;
	
public:
	UBCIVNDKMovieSceneTrack();

	// UMovieSceneTrack interface
	virtual void AddSection(UMovieSceneSection& Section) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	virtual UMovieSceneSection* CreateNewSection() override;
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;
	virtual bool HasSection(const UMovieSceneSection& Section) const override;
	virtual bool IsEmpty() const override;
	virtual void RemoveAllAnimationData() override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;
	virtual void RemoveSectionAt(int32 SectionIndex) override;
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;
	virtual void PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const override;
	virtual bool SupportsMultipleRows() const override { return true; }
	virtual FMovieSceneTrackSegmentBlenderPtr GetTrackSegmentBlender() const override;

#if WITH_EDITOR
	virtual FText GetDefaultDisplayName() const override;
#endif

private:
	
	/** If events should be fired when passed playing the sequence forwards. */
	UPROPERTY(EditAnywhere, Category=TrackEvent, meta=(AllowPrivateAccess=true))
	uint32 bFireEventsWhenForwards:1;

	/** If events should be fired when passed playing the sequence backwards. */
	UPROPERTY(EditAnywhere, Category=TrackEvent, meta=(AllowPrivateAccess=true))
	uint32 bFireEventsWhenBackwards:1;

	/** Defines where in the evaluation to trigger events */
	UPROPERTY(EditAnywhere, Category=TrackEvent, meta=(AllowPrivateAccess=true))
	EFireEventsAtPosition EventPosition;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<class UMovieSceneSection>> CustomSections;
	
	/** The track's sections. */
	UPROPERTY()
	TArray<class UMovieSceneSection*> Sections;
};
