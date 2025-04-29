// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "ISequencerSection.h"

class FBCIVNDKSequencerSection : public FSequencerSection, public TSharedFromThis<FBCIVNDKSequencerSection>
{
public:
	FBCIVNDKSequencerSection(class UMovieSceneSection& InSectionObject, TWeakPtr<class ISequencer> InSequencer);

	virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override;

	virtual void GenerateSectionLayout( ISectionLayoutBuilder& LayoutBuilder ) override;

	virtual int32 OnPaintSection(class FSequencerSectionPainter& Painter) const override;
	
protected:
	static void PaintEventName(class FSequencerSectionPainter& Painter, int32 LayerId, const FString& EventString, float PixelPosition, bool bIsEventValid = true);

	bool IsSectionSelected() const;
	
	TWeakPtr<class ISequencer> Sequencer;
};
