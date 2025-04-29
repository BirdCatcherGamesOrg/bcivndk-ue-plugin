// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "AnimatedGlyphState.generated.h"

USTRUCT()
struct FBCIVNDKWidgetAnimatedGlyphState
{
	GENERATED_BODY()
	
	UPROPERTY()
	double GlyphAnimationProgress = 0.0;

	UPROPERTY()
	double OutlineOpacity = 0.0;

	UPROPERTY()
	double ShadowOpacity = 0.0;
	
	UPROPERTY()
	double GlyphAnimationStartTimestamp = 0.0;
};