// SPDX-License-Identifier: MPL-2.0

#include "BCIVNDKSequencer.h"

#include "BCIVNDKSequencerLogCategory.h"
#include "ControlSceneChannel.h"
#include "ISequencerModule.h"
#include "TrackEditor.h"
#include "SequencerChannelInterface.h"
#include "BCILog.h"

#define LOCTEXT_NAMESPACE "FBCIVNDKSequencerModule"

void FBCIVNDKSequencerModule::StartupModule()
{
	BCILOG_STARTUP_MODULE(LogBCIVNDKSequencer, FBCIVNDKSequencerModule);
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
	
	CreateTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FBCIVNDKSequencerModule::CreateTrackEditor));
	
	SequencerModule.RegisterChannelInterface<FBCIVNDKMovieSceneControlChannel>();
}

void FBCIVNDKSequencerModule::ShutdownModule()
{
	BCILOG_SHUTDOWN_MODULE(LogBCIVNDKSequencer, FBCIVNDKSequencerModule);
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(CreateTrackEditorHandle);
}

TSharedRef<ISequencerTrackEditor> FBCIVNDKSequencerModule::CreateTrackEditor(TSharedRef<ISequencer> Sequencer)
{
	return MakeShared<FBCIVNDKSequencerTrackEditor>(Sequencer);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBCIVNDKSequencerModule, BCIVNDKSequencer)