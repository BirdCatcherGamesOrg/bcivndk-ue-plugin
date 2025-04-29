// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "FunctionReference.h"
#include "MovieSceneMarkedFrame.h"
#include "UserData.h"
#include "Payload.generated.h"

USTRUCT()
struct FBCIVNDKControlDecision
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle Decision;
	
};

USTRUCT()
struct FBCIVNDKControlJump
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FMovieSceneMarkedFrame JumpToFrame;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere)
	FBCICoreFunctionReference Condition;
	
};

USTRUCT()
struct FBCIVNDKControlPayload
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FBCIVNDKControlJump> Jumps;
	
	UPROPERTY(EditAnywhere)
	TArray<FBCIVNDKControlDecision> Decisions;
	
	UPROPERTY(EditAnywhere)
	bool bPause = true;
		
	UPROPERTY(EditAnywhere)
	FBCICoreUserData UserData;
	
};

inline bool operator==(const FBCIVNDKControlPayload& A, const FBCIVNDKControlPayload& B) { return false; }
inline bool operator<(const FBCIVNDKControlPayload& A, const FBCIVNDKControlPayload& B) { return false; }
