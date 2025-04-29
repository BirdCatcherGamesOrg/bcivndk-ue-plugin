// SPDX-License-Identifier: MPL-2.0

#include "AnimatedGlyphRun.h"

#include "AnimatedGlyphState.h"
#include "Framework/Text/ShapedTextCache.h"

FBCIVNDKWidgetAnimatedGlyphRun::FBCIVNDKWidgetAnimatedGlyphRun(const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& InStyle, const FTextRange& InRange, const TArrayView<FBCIVNDKWidgetAnimatedGlyphState>& InGlyphStateView)
	: FSlateTextRun(InRunInfo, InText, InStyle, InRange)
{
}

int32 FBCIVNDKWidgetAnimatedGlyphRun::OnPaint(const FPaintArgs& PaintArgs, const FTextArgs& TextArgs, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return LayerId;
}
