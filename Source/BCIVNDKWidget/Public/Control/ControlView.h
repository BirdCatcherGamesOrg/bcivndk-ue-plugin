// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Blueprint/UserWidget.h"
#include "ControlView.generated.h"

/**
 * @brief UBCIVNDKWidgetControlView is the basic set of widgets for a top level view. 
 */
UCLASS(MinimalAPI)
class UBCIVNDKWidgetControlView : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief ControlButtonBarUISlot is the slot to hold the playback button bar.
	 */
	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> ControlButtonBarUISlot;
	
	/**
	 * @brief EscapeMenuButtonSlot is the slot that holds the esc menu button.
	 * 
	 * In other words, the overlay slot that would appear if you hit escape, pauses the game, etc.
	 * Not used very often.
	 */
	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> EscapeMenuButtonSlot;

	/**
	 * @brief MenuUISlot is the slot for going to the main menu.
	 */
	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> MenuUISlot;
	
};
