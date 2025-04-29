// SPDX-License-Identifier: MPL-2.0

#include "Section.h"

#include "ISectionLayoutBuilder.h"
#include "ISequencer.h"
#include "ISequencerChannelInterface.h"
#include "ISequencerModule.h"
#include "MovieSceneTrack.h"
#include "SequencerSectionPainter.h"
#include "TimeToPixel.h"
#include "Fonts/FontMeasure.h"
#include "MVVM/ViewModels/ChannelModel.h"
#include "Sections/MovieSceneEventSection.h"

/** Data pertaining to a group of channels */
struct FGroupData
{
	FGroupData(FText InGroupText, FGetMovieSceneTooltipText InGetGroupTooltipTextDelegate)
		: GroupText(InGroupText)
		, GetGroupTooltipTextDelegate(InGetGroupTooltipTextDelegate)
		, SortOrder(-1)
		, bSortEmptyGroupsLast(true)
	{}

	void AddChannel(ISequencerSection::FChannelData&& InChannel)
	{
		if (InChannel.MetaData.SortOrder < SortOrder)
		{
			SortOrder = InChannel.MetaData.SortOrder;
		}
		bSortEmptyGroupsLast = InChannel.MetaData.bSortEmptyGroupsLast;

		Channels.Add(MoveTemp(InChannel));
	}

	/** Text to display for the group */
	FText GroupText;
	
	/** Getter for text to display for the group tooltip */
	FGetMovieSceneTooltipText GetGroupTooltipTextDelegate;

	/** Sort order of the group */
	uint32 SortOrder;

	/** By default if a channel has no FText::Group specified, we put it last, by setting this to false we use SortIndex instead */
	bool bSortEmptyGroupsLast;

	/** Array of channels within this group */
	TArray<ISequencerSection::FChannelData, TInlineAllocator<4>> Channels;
};

FBCIVNDKSequencerSection::FBCIVNDKSequencerSection(UMovieSceneSection& InSectionObject, TWeakPtr<ISequencer> InSequencer)
	: FSequencerSection(InSectionObject)
	, Sequencer(InSequencer)
{
}

void FBCIVNDKSequencerSection::BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding)
{
	MenuBuilder.BeginSection(NAME_None, FText::FromString("View"));
	{
		MenuBuilder.AddMenuEntry(
			FText::FromString("Control Trigger"),
			FText::FromString("AddNewTriggerSectionTooltip"),
			FSlateIcon(),
			FUIAction()
		);
	}
	MenuBuilder.EndSection();
}

void FBCIVNDKSequencerSection::GenerateSectionLayout(ISectionLayoutBuilder& LayoutBuilder)
{
	using namespace UE::Sequencer;

	UMovieSceneSection* Section = GetSectionObject();
	if (!Section)
	{
		return;
	}

	// Group channels by their group name
	TMap<FName,  FGroupData> GroupToChannelsMap;

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	for (const FMovieSceneChannelEntry& Entry : Section->GetChannelProxy().GetAllEntries())
	{
		const FName ChannelTypeName = Entry.GetChannelTypeName();

		// One editor data ptr per channel
		TArrayView<FMovieSceneChannel* const>        Channels    = Entry.GetChannels();
		TArrayView<const FMovieSceneChannelMetaData> AllMetaData = Entry.GetMetaData();

		for (int32 Index = 0; Index < Channels.Num(); ++Index)
		{
			FMovieSceneChannelHandle Channel = ChannelProxy.MakeHandle(ChannelTypeName, Index);

			const FMovieSceneChannelMetaData& MetaData = AllMetaData[Index];
			if (MetaData.bEnabled)
			{
				FName GroupName = *MetaData.Group.ToString();

				FGroupData* ExistingGroup = GroupToChannelsMap.Find(GroupName);
				if (!ExistingGroup)
				{
					FText GroupDisplayName = FText::FromString(MetaData.GetPropertyMetaData(FCommonChannelData::GroupDisplayName));
					if (GroupDisplayName.IsEmpty())
					{
						GroupDisplayName = FText::FromName(GroupName);
					}
					ExistingGroup = &GroupToChannelsMap.Add(GroupName, FGroupData(GroupDisplayName, MetaData.GetGroupTooltipTextDelegate));
				}

				ExistingGroup->AddChannel(FChannelData{ Channel, MetaData });
			}
		}
	}

	if (GroupToChannelsMap.Num() == 0)
	{
		return;
	}


	ISequencerModule* SequencerModule = &FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");

	auto ChannelFactory = [this, SequencerModule](FName InChannelName, const FSectionModel& InSection, const FMovieSceneChannelHandle& InChannel)
	{
		TSharedPtr<FChannelModel> ChannelModel = this->ConstructChannelModel(InChannelName, InChannel);
		if (!ChannelModel)
		{
			ISequencerChannelInterface* EditorInterface = SequencerModule->FindChannelEditorInterface(InChannel.GetChannelTypeName());
			if (EditorInterface)
			{
				ChannelModel = EditorInterface->CreateChannelModel_Raw(InChannel, InSection, InChannelName);
			}
		}

		return ChannelModel;
	};

	// Collapse single channels to the top level track node if allowed
	if (GroupToChannelsMap.Num() == 1)
	{
		const TTuple<FName, FGroupData>& Pair = *GroupToChannelsMap.CreateIterator();
		if (Pair.Value.Channels.Num() == 1 && Pair.Value.Channels[0].MetaData.bCanCollapseToTrack)
		{
			LayoutBuilder.SetTopLevelChannel(Pair.Value.Channels[0].Channel, ChannelFactory);
			return;
		}
	}

	// Sort the channels in each group by its sort order and name
	TArray<FName, TInlineAllocator<6>> SortedGroupNames;
	for (TPair<FName, FGroupData>& Pair : GroupToChannelsMap)
	{
		SortedGroupNames.Add(Pair.Key);

		// Sort by sort order then name
		Pair.Value.Channels.Sort([](const FChannelData& A, const FChannelData& B){
			if (A.MetaData.SortOrder == B.MetaData.SortOrder)
			{
				return A.MetaData.Name.LexicalLess(B.MetaData.Name);
			}
			return A.MetaData.SortOrder < B.MetaData.SortOrder;
		});
	}

	// Sort groups by the lowest sort order in each group
	auto SortPredicate = [&GroupToChannelsMap](FName A, FName B)
	{
		if (A.IsNone())
		{
			const bool bSortEmptyGroupsLast = GroupToChannelsMap.FindChecked(A).bSortEmptyGroupsLast;
			if(bSortEmptyGroupsLast)
			{ 
				return false;
			}
		}
		else if (B.IsNone())
		{
			const bool bSortEmptyGroupsLast = GroupToChannelsMap.FindChecked(B).bSortEmptyGroupsLast;
			if (bSortEmptyGroupsLast)
			{
				return true;
			}
		}


		const int32 SortOrderA = GroupToChannelsMap.FindChecked(A).SortOrder;
		const int32 SortOrderB = GroupToChannelsMap.FindChecked(B).SortOrder;
		return SortOrderA < SortOrderB;
	};
	SortedGroupNames.Sort(SortPredicate);


	// Create key areas for each group name
	for (FName GroupName : SortedGroupNames)
	{
		FGroupData& ChannelData = GroupToChannelsMap.FindChecked(GroupName);

		if (!GroupName.IsNone())
		{
			auto Factory = [this, &ChannelData](FName InCategoryName, const FText& InDisplayText)
			{
				return this->ConstructCategoryModel(InCategoryName, InDisplayText, ChannelData.Channels);
			};

			LayoutBuilder.PushCategory(GroupName, ChannelData.GroupText, ChannelData.GetGroupTooltipTextDelegate, Factory);
		}

		for (const FChannelData& ChannelAndData : ChannelData.Channels)
		{
			LayoutBuilder.AddChannel(ChannelAndData.Channel, ChannelFactory);
		}

		if (!GroupName.IsNone())
		{
			LayoutBuilder.PopCategory();
		}
	}
}

int32 FBCIVNDKSequencerSection::OnPaintSection(class FSequencerSectionPainter& Painter) const
{
	const int32 LayerId = Painter.PaintSectionBackground();
	UMovieSceneEventSection* EventSection = Cast<UMovieSceneEventSection>(WeakSection.Get());
	if (!EventSection)
	{
		return LayerId;
	}

	const FTimeToPixel& TimeToPixelConverter = Painter.GetTimeConverter();

	for (int32 KeyIndex = 0; KeyIndex < EventSection->GetEventData().GetKeyTimes().Num(); ++KeyIndex)
	{
		FFrameNumber EventTime = EventSection->GetEventData().GetKeyTimes()[KeyIndex];
		FEventPayload EventData = EventSection->GetEventData().GetKeyValues()[KeyIndex];

		if (EventSection->GetRange().Contains(EventTime))
		{
			FString EventString = EventData.EventName.ToString();
			if (!EventString.IsEmpty())
			{
				const float PixelPos = TimeToPixelConverter.FrameToPixel(EventTime);
				PaintEventName(Painter, LayerId, EventString, PixelPos);
			}
		}
	}

	return LayerId + 3;
}

bool FBCIVNDKSequencerSection::IsSectionSelected() const
{
	TSharedPtr<ISequencer> SequencerPtr = Sequencer.Pin();

	TArray<UMovieSceneTrack*> SelectedTracks;
	SequencerPtr->GetSelectedTracks(SelectedTracks);

	UMovieSceneSection* Section = WeakSection.Get();
	UMovieSceneTrack* Track = Section ? CastChecked<UMovieSceneTrack>(Section->GetOuter()) : nullptr;
	return Track && SelectedTracks.Contains(Track);
}

void FBCIVNDKSequencerSection::PaintEventName(FSequencerSectionPainter& Painter, int32 LayerId, const FString& InEventString, float PixelPos, bool bIsEventValid)
{
	static constexpr float BoxOffsetPx = 10.f;
	static const TCHAR* WarningString = TEXT("\xf071");

	const FSlateFontInfo FontAwesomeFont = FAppStyle::GetFontStyle("FontAwesome.10");
	const FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Bold", 10);
	const FLinearColor DrawColor = FAppStyle::GetSlateColor("SelectionColor").GetColor(FWidgetStyle()).CopyWithNewOpacity(1.0f);

	TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();

	// Setup the warning size. Static since it won't ever change
	static FVector2D WarningSize = FontMeasureService->Measure(WarningString, FontAwesomeFont);
	const FMargin WarningPadding = bIsEventValid || InEventString.Len() == 0 ? FMargin(0.f) : FMargin(0.f, 0.f, 4.f, 0.f);
	const FMargin BoxPadding = FMargin(4.0f, 2.0f);

	const FVector2D TextSize = FontMeasureService->Measure(InEventString, SmallLayoutFont);
	const FVector2D IconSize = bIsEventValid ? FVector2D::ZeroVector : WarningSize;
	const FVector2D PaddedIconSize = IconSize + WarningPadding.GetDesiredSize();
	const FVector2D BoxSize = FVector2D(TextSize.X + PaddedIconSize.X, FMath::Max(TextSize.Y, PaddedIconSize.Y)) + BoxPadding.GetDesiredSize();

	// Flip the text position if getting near the end of the view range
	bool bDrawLeft = Painter.SectionGeometry.Size.X - PixelPos < BoxSize.X + 22.f - BoxOffsetPx;
	float BoxPositionX = bDrawLeft ? PixelPos - BoxSize.X - BoxOffsetPx : PixelPos + BoxOffsetPx;
	if (BoxPositionX < 0.f)
	{
		BoxPositionX = 0.f;
	}

	FVector2D BoxOffset = FVector2D(BoxPositionX, Painter.SectionGeometry.Size.Y * .5f - BoxSize.Y * .5f);
	FVector2D IconOffset = FVector2D(BoxPadding.Left, BoxSize.Y * .5f - IconSize.Y * .5f);
	FVector2D TextOffset = FVector2D(IconOffset.X + PaddedIconSize.X, BoxSize.Y * .5f - TextSize.Y * .5f);

	// Draw the background box
	FSlateDrawElement::MakeBox(
		Painter.DrawElements,
		LayerId + 1,
		Painter.SectionGeometry.ToPaintGeometry(BoxSize, FSlateLayoutTransform(BoxOffset)),
		FAppStyle::GetBrush("WhiteBrush"),
		ESlateDrawEffect::None,
		FLinearColor::Black.CopyWithNewOpacity(0.5f)
	);

	if (!bIsEventValid)
	{
		// Draw a warning icon for unbound repeaters
		FSlateDrawElement::MakeText(
			Painter.DrawElements,
			LayerId + 2,
			Painter.SectionGeometry.ToPaintGeometry(IconSize, FSlateLayoutTransform(BoxOffset + IconOffset)),
			WarningString,
			FontAwesomeFont,
			Painter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
			FAppStyle::GetWidgetStyle<FTextBlockStyle>("Log.Warning").ColorAndOpacity.GetSpecifiedColor()
		);
	}
	
	FSlateDrawElement::MakeText(
		Painter.DrawElements,
		LayerId + 2,
		Painter.SectionGeometry.ToPaintGeometry(TextSize, FSlateLayoutTransform(BoxOffset + TextOffset)),
		InEventString,
		SmallLayoutFont,
		Painter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
		DrawColor
	);
}
