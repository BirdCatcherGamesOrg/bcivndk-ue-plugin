// SPDX-License-Identifier: MPL-2.0

#include "AnimatedGlyphDecorator.h"

#include "AnimatedGlyphState.h"
#include "AnimatedGlyphRun.h"
#include "Framework/Text/SlateTextRun.h"

bool FBCIVNDKWidgetAnimatedGlyphDecorator::Supports(const FTextRunParseResults& RunInfo, const FString& Text) const
{
	return RunInfo.Name.IsEmpty() && !Text.IsEmpty();
}

TSharedRef<ISlateRun> FBCIVNDKWidgetAnimatedGlyphDecorator::Create(const TSharedRef<FTextLayout>& TextLayout,
	const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText,
	const ISlateStyle* Style)
{
	FRunInfo RunInfo(RunParseResult.Name);
	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}
	
	FTextRange ModelRange;
	ModelRange.BeginIndex = TextRunsToDecorate.Last()->GetTextRange().EndIndex;
	FString RunText = OriginalText.Mid(RunParseResult.OriginalRange.BeginIndex, RunParseResult.OriginalRange.EndIndex - RunParseResult.OriginalRange.BeginIndex);
	ModelRange.EndIndex = ModelRange.BeginIndex + RunText.Len();
	*InOutModelText += RunText;
	
	TArrayView<FBCIVNDKWidgetAnimatedGlyphState> GlyphStatesView =
		MakeArrayView(GlyphStates.GetData() +  GlyphStates.Num(), RunText.Len());
	TSharedRef<FBCIVNDKWidgetAnimatedGlyphRun> Run =
		MakeShared<FBCIVNDKWidgetAnimatedGlyphRun>(RunInfo, InOutModelText, TextBlockStyle, ModelRange, GlyphStatesView);
	TextRunsToDecorate.Add(Run);
	
	return Run;
}

bool FBCIVNDKWidgetAnimatedGlyphDecorator::Start()
{
	CurrentGlyphAnimationTime = 0.0;
	if (CharactersPerSecond < 0)
	{
		// Jump to the end of the current text run? 
	}
	
	FTickerDelegate GlyphAnimation = FTickerDelegate::CreateSPLambda(this, [WeakThis = AsWeak()](float Delta)
	{
		TSharedPtr<FBCIVNDKWidgetAnimatedGlyphDecorator> SharedThis = WeakThis.Pin();
		if (!SharedThis)
		{
			return false;
		}
		// Use GlyphStateChangeTimestamp, Delta, and CharactersPerSecond to determine number of letters to animate.
		SharedThis->GlyphStates;
		SharedThis->CurrentGlyphAnimationTime += Delta;
		return true;
	});
	
	GlyphAnimationHandle = FTSTicker::GetCoreTicker().AddTicker(GlyphAnimation);
	return true;
}

void FBCIVNDKWidgetAnimatedGlyphDecorator::Pause()
{
	FTSTicker::GetCoreTicker().RemoveTicker(GlyphAnimationHandle);
	GlyphAnimationHandle.Reset();
}

bool FBCIVNDKWidgetAnimatedGlyphDecorator::ShowTo(const int32& GlyphIndex)
{
	if (GlyphIndex >= GlyphStates.Num())
	{
		return false;
	}

	CurrentGlyphIndex = GlyphIndex;
	for (int i = 0; i < GlyphStates.Num(); i++)
	{
		GlyphStates[i].GlyphAnimationStartTimestamp = -1.0;
		GlyphStates[i].GlyphAnimationProgress = i < GlyphIndex ? 1.0 : 0.0;
	}
	
	return true;
}
