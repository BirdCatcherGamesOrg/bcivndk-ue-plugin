// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

class FBCIVNDKSequencerTrackEditor : public FMovieSceneTrackEditor
{
public:
	
	FBCIVNDKSequencerTrackEditor(TSharedRef<ISequencer> InSequencer);
	
	// ISequencerTrackEditor interface
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	virtual bool SupportsSequence(UMovieSceneSequence* InSequence) const override;
	virtual const FSlateBrush* GetIconBrush() const override;
	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;
	
	// FPropertyTrackEditor interface
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;

private:

	/** Callback for executing the "Add Event Track" menu entry. */
	void HandleAddTrackMenuEntryExecute();

	void CreateNewSection(UMovieSceneTrack* Track, int32 RowIndex, UClass* SectionType, bool bSelect);

};