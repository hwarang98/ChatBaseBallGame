// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseBallGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALL_API ABaseBallGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseBallGameModeBase();
	bool IsValidGuess(const FString& Guess);
	FString CheckGuess(const FString& Guess);
	void CheckGameEnd();
	void ResetGame();

protected:
	void BeginPlay() override;
	
private:
	TArray<int32> AnswerNumbers; // 정답 숫자
	
	void GenerateAnswerNumbers();
};
