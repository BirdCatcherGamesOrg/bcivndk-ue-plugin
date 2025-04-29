// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/SlateTextRun.h"

class FBCIVNDKWidgetAnimatedGlyphRun : public FSlateTextRun
{
public:

	FBCIVNDKWidgetAnimatedGlyphRun(const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& InStyle, const FTextRange& InRange, const TArrayView<struct FBCIVNDKWidgetAnimatedGlyphState>& InGlyphStateView);

	virtual int32 OnPaint(const FPaintArgs& PaintArgs, const FTextArgs& TextArgs, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:

	TArrayView<struct FBCIVNDKWidgetAnimatedGlyphState> GlyphStateView;
};