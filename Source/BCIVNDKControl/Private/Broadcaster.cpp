// SPDX-License-Identifier: MPL-2.0

#include "Broadcaster.h"

UBCIVNDKControlBroadcaster::UBCIVNDKControlBroadcaster()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBCIVNDKControlBroadcaster::Handle(const FBCIVNDKControlBroadcast& Broadcast)
{
	if (HandleControl.IsBound())
	{
		const bool bBlockOtherChannels = HandleControl.Execute(Broadcast.ControlPayload);
		if (bBlockOtherChannels)
		{
			return;
		}
	}
	
	for (const auto Field : Broadcast.TokenValues.JsonObject->Values)
	{
		FHandleChannelDataSignature* OnHandleChannelData = HandleChannelDataSignatures.Find(Field.Key);
		if (OnHandleChannelData)
		{
			TSharedPtr<FJsonObject>* Object;
			const bool bHasObject = Field.Value->TryGetObject(Object);
			if (bHasObject)
			{
				const bool bExecuted = OnHandleChannelData->ExecuteIfBound(Object->ToSharedRef());
			}
		}
	}
}

bool UBCIVNDKControlBroadcaster::OnHandleControl(FHandleControlSignature&& Signature)
{
	if (HandleControl.IsBound())
	{
		return false;
	}
	HandleControl = MoveTemp(Signature);
	return true;
}
