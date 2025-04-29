// SPDX-License-Identifier: MPL-2.0

#include "Control/ControlModelComponent.h"

#include "Components/NamedSlot.h"

UBCIVNDKWidgetControlModelComponent::UBCIVNDKWidgetControlModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UBCIVNDKWidgetControlModelComponent::CreateView(UNamedSlot& Slot)
{
}

UBCIVNDKWidgetControlView* UBCIVNDKWidgetControlModelComponent::GetControlView() const
{
	return Widget;
}
