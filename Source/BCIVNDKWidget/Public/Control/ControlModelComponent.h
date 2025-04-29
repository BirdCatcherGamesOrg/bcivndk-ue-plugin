// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "ControlModelComponent.generated.h"

UCLASS(MinimalApi)
class UBCIVNDKWidgetControlModelComponent : public UActorComponent
{

	GENERATED_BODY()
	
public:

	UBCIVNDKWidgetControlModelComponent();

	// UTextureRenderTarget2D to render the avatar onto. Can be a static image, or it could be a camera or w/e.

	void CreateView(class UNamedSlot& Slot);

	class UBCIVNDKWidgetControlView* GetControlView() const;
	
private:
	
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UBCIVNDKWidgetControlView> ControlWidgetType;

	UPROPERTY(Transient)
	TObjectPtr<class UBCIVNDKWidgetControlView> Widget;
	
};
