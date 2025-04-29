// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JsonObjectWrapper.h"
#include "Payload.h"
#include "Broadcaster.generated.h"

USTRUCT()
struct FBCIVNDKControlBroadcast
{
	GENERATED_BODY()
	
	UPROPERTY()
	FFrameNumber Time;
	
	UPROPERTY()
	FBCIVNDKControlPayload ControlPayload;

	UPROPERTY()
	FJsonObjectWrapper TokenValues;
	
};

inline bool operator==(const FBCIVNDKControlBroadcast& A, const FBCIVNDKControlBroadcast& B) { return A.Time == B.Time; }
inline bool operator<(const FBCIVNDKControlBroadcast& A, const FBCIVNDKControlBroadcast& B) { return A.Time < B.Time; }

USTRUCT()
struct FBCIVNDKHandleChannelDataParam
{
	GENERATED_BODY()

	FBCIVNDKHandleChannelDataParam() = default;
	FBCIVNDKHandleChannelDataParam(const TSharedRef<FJsonObject>& InJsonObject) : JsonObject(InJsonObject) {};
	
	TSharedRef<FJsonObject> JsonObject;
	
};

UCLASS(MinimalAPI, meta=(BlueprintSpawnableComponent))
class UBCIVNDKControlBroadcaster : public UActorComponent
{
	GENERATED_BODY()

	friend struct FBCIVNDKMovieSceneExecutionToken;
	
public:

	DECLARE_DELEGATE_RetVal_OneParam(bool /* bBlockOtherChannels */, FHandleControlSignature, const FBCIVNDKControlPayload& /* Param */);
	
	DECLARE_DELEGATE_OneParam(FHandleChannelDataSignature, const FBCIVNDKHandleChannelDataParam& /* Param */);
	
	// Sets default values for this component's properties
	UBCIVNDKControlBroadcaster();
	
	BCIVNDKCONTROL_API bool OnHandleControl(FHandleControlSignature&& Signature);

	template
	<typename ChannelDataType>
	FHandleChannelDataSignature& OnHandleChannelData();

private:

	BCIVNDKCONTROL_API void Handle(const FBCIVNDKControlBroadcast& Broadcast);
	
	TMap<FString, FHandleChannelDataSignature> HandleChannelDataSignatures;
	
	FHandleControlSignature HandleControl;
};

template <typename ChannelDataType>
UBCIVNDKControlBroadcaster::FHandleChannelDataSignature& UBCIVNDKControlBroadcaster::OnHandleChannelData()
{
	return HandleChannelDataSignatures.FindOrAdd(ChannelDataType::StaticStruct()->GetName());
}
