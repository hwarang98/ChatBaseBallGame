// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseBallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALL_API ABaseBallPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	void IncreaseAttempt() { CurrentAttempt++; }
	int32 GetCurrentAttempt() const { return CurrentAttempt; }
	int32 GetMaxAttempt() const { return MaxAttempt; }
	FString GetLastResult() const { return LastResult; }
	void ResetAttempts() { CurrentAttempt = 0; LastResult = TEXT(""); }
	void SetLastResult(const FString& NewResult) { LastResult = NewResult; }

private:
	int32 CurrentAttempt;
	const int32 MaxAttempt = 3;
	FString LastResult;

};
