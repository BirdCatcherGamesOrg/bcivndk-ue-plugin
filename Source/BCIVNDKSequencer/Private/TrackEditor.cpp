// SPDX-License-Identifier: MPL-2.0

#include "TrackEditor.h"

#include "ControlSection.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ISequencerSection.h"
#include "MovieSceneSequenceEditor.h"
#include "SequencerUtilities.h"
#include "Track.h"
#include "LevelSequence.h"
#include "Section.h"

#define LOCTEXT_NAMESPACE "BCIVNDKSequencer"

FBCIVNDKSequencerTrackEditor::FBCIVNDKSequencerTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
	
}

TSharedRef<ISequencerSection> FBCIVNDKSequencerTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	if (SectionObject.IsA<UBCIVNDKMovieSceneControlSection>())
	{
		return MakeShared<FBCIVNDKSequencerSection>(SectionObject, GetSequencer());
	}

	return MakeShared<FSequencerSection>(SectionObject);
}

void FBCIVNDKSequencerTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();
	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(RootMovieSceneSequence);

	if (SequenceEditor && SequenceEditor->SupportsEvents(RootMovieSceneSequence))
	{
		MenuBuilder.AddMenuEntry(
		LOCTEXT("AddTrack", "BCIVNDK Track"),
		LOCTEXT("AddTooltip", "Adds a new BCIVNDK track."),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Sequencer.Tracks.Event"),
			FUIAction(
				FExecuteAction::CreateRaw(this, &FBCIVNDKSequencerTrackEditor::HandleAddTrackMenuEntryExecute)
			)
		);
	}
}

TSharedPtr<SWidget> FBCIVNDKSequencerTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	check(Track);

	const TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	if (!SequencerPtr.IsValid())
	{
		return SNullWidget::NullWidget;
	}

	TWeakObjectPtr<UMovieSceneTrack> WeakTrack = Track;
	const int32 RowIndex = Params.TrackInsertRowIndex;
	auto SubMenuCallback = [this, WeakTrack, RowIndex]
	{
		FMenuBuilder MenuBuilder(true, nullptr);

		UMovieSceneTrack* TrackPtr = WeakTrack.Get();
		if (TrackPtr)
		{
			UBCIVNDKMovieSceneTrack* DialogueLinesSceneTrack = Cast<UBCIVNDKMovieSceneTrack>(TrackPtr);
			for (const TSubclassOf<UMovieSceneSection>& CustomSection : DialogueLinesSceneTrack->CustomSections)
			{
				if (CustomSection)
				{
					MenuBuilder.AddMenuEntry(
						LOCTEXT("AddNewSection", "Add."),
						LOCTEXT("AddNewSectionTooltip", "Adds a new."),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateSP(SharedThis(this), &FBCIVNDKSequencerTrackEditor::CreateNewSection, TrackPtr, RowIndex + 1, CustomSection.Get(), true))
					);
				}
			}}
		else
		{
			MenuBuilder.AddWidget(SNew(STextBlock).Text(LOCTEXT("InvalidTrack", "Track is no longer valid")), FText(), true);
		}

		return MenuBuilder.MakeWidget();
	};

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			FSequencerUtilities::MakeAddButton(LOCTEXT("AddSection", "Section"), FOnGetContent::CreateLambda(SubMenuCallback), Params.NodeIsHovered, GetSequencer())
		];
}

bool FBCIVNDKSequencerTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UBCIVNDKMovieSceneTrack::StaticClass();
}

bool FBCIVNDKSequencerTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	return InSequence && InSequence->GetClass()->IsChildOf(ULevelSequence::StaticClass());
}

const FSlateBrush* FBCIVNDKSequencerTrackEditor::GetIconBrush() const
{
	return FAppStyle::GetBrush("Sequencer.Tracks.Event");
}

void FBCIVNDKSequencerTrackEditor::HandleAddTrackMenuEntryExecute()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	if (FocusedMovieScene->IsReadOnly())
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("HandleAddTrackMenuEntryExecute_Transaction", "HandleAddTrackMenuEntryExecute"));
	FocusedMovieScene->Modify();

	TArray<UBCIVNDKMovieSceneTrack*> NewTracks;

	UBCIVNDKMovieSceneTrack* NewMasterTrack = FocusedMovieScene->AddTrack<UBCIVNDKMovieSceneTrack>();
	NewTracks.Add(NewMasterTrack);
	if (GetSequencer().IsValid())
	{
		GetSequencer()->OnAddTrack(NewMasterTrack, FGuid());
	}

	check(NewTracks.Num() != 0);

	for (UBCIVNDKMovieSceneTrack* NewTrack : NewTracks)
	{
		int32 RowIndex = 0;
		CreateNewSection(NewTrack, RowIndex, UBCIVNDKMovieSceneControlSection::StaticClass(), false);
		for (const TSubclassOf<UMovieSceneSection>& CustomSection : NewTrack->CustomSections)
		{
			RowIndex++;
			CreateNewSection(NewTrack, RowIndex, CustomSection, false);
		}
		NewTrack->SetDisplayName(LOCTEXT("TrackName", "BCIVNDK Track"));
	}
}

void FBCIVNDKSequencerTrackEditor::CreateNewSection(UMovieSceneTrack* Track, int32 RowIndex, UClass* SectionType, bool bSelect)
{
	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	if (SequencerPtr.IsValid())
	{
		UMovieScene* FocusedMovieScene = GetFocusedMovieScene();
		const FQualifiedFrameTime CurrentTime = SequencerPtr->GetLocalTime();

		FScopedTransaction Transaction(LOCTEXT("CreateNewSection_Transaction", "CreateNewSection"));

		UMovieSceneSection* NewSection = NewObject<UMovieSceneSection>(Track, SectionType);
		check(NewSection);

		int32 OverlapPriority = 0;
		for (UMovieSceneSection* Section : Track->GetAllSections())
		{
			if (Section->GetRowIndex() >= RowIndex)
			{
				Section->SetRowIndex(Section->GetRowIndex() + 1);
			}
			OverlapPriority = FMath::Max(Section->GetOverlapPriority() + 1, OverlapPriority);
		}

		Track->Modify();

		if (SectionType == UBCIVNDKMovieSceneControlSection::StaticClass())
		{
			NewSection->SetRange(TRange<FFrameNumber>::All());
		}
		else
		{
			TRange<FFrameNumber> NewSectionRange;

			if (CurrentTime.Time.FrameNumber < FocusedMovieScene->GetPlaybackRange().GetUpperBoundValue())
			{
				NewSectionRange = TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, FocusedMovieScene->GetPlaybackRange().GetUpperBoundValue());
			}
			else
			{
				constexpr float DefaultLengthInSeconds = 5.f;
				NewSectionRange = TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, CurrentTime.Time.FrameNumber + (DefaultLengthInSeconds * SequencerPtr->GetFocusedTickResolution()).FloorToFrame());
			}

			NewSection->SetRange(NewSectionRange);
		}

		NewSection->SetOverlapPriority(OverlapPriority);
		NewSection->SetRowIndex(RowIndex);

		Track->AddSection(*NewSection);
		Track->UpdateEasing();

		if (bSelect)
		{
			SequencerPtr->EmptySelection();
			SequencerPtr->SelectSection(NewSection);
			SequencerPtr->ThrobSectionSelection();
		}

		SequencerPtr->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	}
}

#undef LOCTEXT_NAMESPACE
