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
	TSharedRef<FBCIVNDKSequencerModule> SharedThis = AsShared();
	CreateTrackEditorHandle = SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateSPLambda(SharedThis, [WeakThis = SharedThis->AsWeak()] (TSharedRef<ISequencer> Sequencer)
	{
		TSharedPtr<FBCIVNDKSequencerModule> SharedThis = WeakThis.Pin();
		if (!SharedThis.IsValid())
		{
			BCILOG(LogBCIVNDKSequencer, Error, "Something went wrong with loading the track editor. I'm not as clever as I think I am with smart pointers.");
			checkNoEntry();
		}
		
		if (!SharedThis->TrackEditor.IsValid())
		{
			SharedThis->TrackEditor = MakeShared<FBCIVNDKSequencerTrackEditor>(Sequencer);
		}
		
		return SharedThis->TrackEditor.ToSharedRef();
	}));
	
	SequencerModule.RegisterChannelInterface<FBCIVNDKMovieSceneControlChannel>();
}

void FBCIVNDKSequencerModule::ShutdownModule()
{
	BCILOG_SHUTDOWN_MODULE(LogBCIVNDKSequencer, FBCIVNDKSequencerModule);
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.UnRegisterTrackEditor(CreateTrackEditorHandle);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBCIVNDKSequencerModule, BCIVNDKSequencer)