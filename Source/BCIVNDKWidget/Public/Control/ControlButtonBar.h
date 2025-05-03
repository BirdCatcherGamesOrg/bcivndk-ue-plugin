// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlButtonBar.generated.h"

/**
 * @brief Buttons to control playback of the VN scene, as well as system things like settings.
 */
UCLASS(MinimalAPI)
class UBCIVNDKWidgetControlButtonBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> AutoButton;	
	
	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> NextButton;

	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> HistoryButton;

	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> QuickSaveButton;

	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> SettingsButton;
	
};
