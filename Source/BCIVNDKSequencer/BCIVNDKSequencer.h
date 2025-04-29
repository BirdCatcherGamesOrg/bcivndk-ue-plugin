// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBCIVNDKSequencerModule : public IModuleInterface, public TSharedFromThis<FBCIVNDKSequencerModule>
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    
    FDelegateHandle CreateTrackEditorHandle;

    static TSharedRef<class ISequencerTrackEditor> CreateTrackEditor(TSharedRef<class ISequencer> Sequencer);
};
