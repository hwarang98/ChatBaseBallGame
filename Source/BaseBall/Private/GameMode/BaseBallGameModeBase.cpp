// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseBallGameModeBase.h"

#include "Controller/BaseBallPlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "State/BaseBallPlayerState.h"

ABaseBallGameModeBase::ABaseBallGameModeBase()
{
}

void ABaseBallGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GenerateAnswerNumbers();
}

void ABaseBallGameModeBase::GenerateAnswerNumbers()
{
	AnswerNumbers.Empty();
	TArray<int32> Numbers = {1,2,3,4,5,6,7,8,9};
	FMath::RandInit(FDateTime::Now().GetTicks());

	for (int i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		AnswerNumbers.Add(Numbers[Index]);
		Numbers.RemoveAt(Index);
	}
}

FString ABaseBallGameModeBase::CheckGuess(const FString& Guess)
{
	int32 S = 0, B = 0;
    
	for(int i = 0; i < 3; ++i)
	{
		int32 GuessDigit = Guess[i] - '0';
		if(GuessDigit == AnswerNumbers[i]) S++;
		else if(AnswerNumbers.Contains(GuessDigit)) B++;
	}

	if(S == 0 && B == 0)
	{
		return "OUT";
	}
	return FString::Printf(TEXT("%dS%dB"), S, B);
}

bool ABaseBallGameModeBase::IsValidGuess(const FString& Guess)
{
	if(Guess.Len() != 3)
	{
		return false;
	}
	
	TSet<TCHAR> UniqueDigits;

	for(TCHAR C : Guess)
	{
		if(!FChar::IsDigit(C) || C == '0')
		{
			return false;
		}
		if(UniqueDigits.Contains(C))
		{
			return false;
		}
		UniqueDigits.Add(C);
	}
	return true;
}

void ABaseBallGameModeBase::CheckGameEnd()
{
	bool bAllUsedUp = true;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (const ABaseBallPlayerState* MyPS = Cast<ABaseBallPlayerState>(PS))
		{
			if (MyPS->GetCurrentAttempt() < MyPS->GetMaxAttempt())
			{
				bAllUsedUp = false;
			}
			if (MyPS->GetLastResult() == TEXT("3S0B"))
			{
				// UE_LOG 대신 공지 메시지를 만듭니다.
				FString VictoryMessage = FString::Printf(TEXT("%s 님이 승리했습니다!"), *MyPS->GetPlayerName());

				// 모든 플레이어에게 공지 RPC를 호출합니다.
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
				{
					if (ABaseBallPlayerController* PC = Cast<ABaseBallPlayerController>(It->Get()))
					{
						PC->MulticastShowNotice(VictoryMessage);
					}
				}
                
				ResetGame();
				return;
			}
		}
	}

	// 모두 횟수를 다 썼다면 무승부
	if (bAllUsedUp)
	{
		UE_LOG(LogTemp, Warning, TEXT("무승부!"));
		ResetGame();
	}
}

void ABaseBallGameModeBase::ResetGame()
{
	GenerateAnswerNumbers();

	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ABaseBallPlayerState* MyPS = Cast<ABaseBallPlayerState>(PS))
		{
			MyPS->ResetAttempts();
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("게임 리셋됨. 새로운 정답 생성 완료"));
}
