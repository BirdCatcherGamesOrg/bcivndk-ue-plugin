// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Framework/Text/ITextDecorator.h"

struct FBCIVNDKWidgetAnimatedGlyphDecorator : ITextDecorator, TSharedFromThis<FBCIVNDKWidgetAnimatedGlyphDecorator>
{
	/**
	 * @brief Supports all text runs that aren't already specialized.
	 */
	virtual bool Supports(const FTextRunParseResults& RunInfo, const FString& Text) const override;

	virtual TSharedRef<ISlateRun> Create(const TSharedRef<FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style) override;

	bool Start();
	
	void Pause();
	
	bool ShowTo(const int32& GlyphIndex);

private:

	FTextBlockStyle TextBlockStyle;
	
	FTSTicker::FDelegateHandle GlyphAnimationHandle;
	
	double CharactersPerSecond = -1.0;
	
	TArray<TSharedRef<class FSlateTextRun>> TextRunsToDecorate;

	TArray<struct FBCIVNDKWidgetAnimatedGlyphState> GlyphStates;

	int32 CurrentGlyphIndex = 0;

	float CurrentGlyphAnimationTime = 0;

};


