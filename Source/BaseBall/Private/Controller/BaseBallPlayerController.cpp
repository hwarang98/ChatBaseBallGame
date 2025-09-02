// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BaseBallPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameMode/BaseBallGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "State/BaseBallPlayerState.h"
#include "Widget/BaseBallUserWidget.h"

void ABaseBallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("BaseBallPlayerController BeginPlay Executed!"));
	}

	if(ChatWidgetClass)
	{
		ChatWidget = CreateWidget<UBaseBallUserWidget>(this, ChatWidgetClass);
		if(ChatWidget)
		{
			ChatWidget->AddToViewport();
			// 1. UI 전용 입력 모드로 설정
			FInputModeUIOnly InputModeData;
			
			// 2. 우리가 만든 채팅 위젯에 키보드 포커스
			//    SetWidgetToFocus는 SWidget을 인자로 받으므로 TakeWidget()으로 변환
			InputModeData.SetWidgetToFocus(ChatWidget->TakeWidget());

			// 3. 마우스 커서가 위젯 밖으로 나가지 않도록 잠글지 여부
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
			// 4. 플레이어 컨트롤러에 새로운 입력 모드를 적용
			SetInputMode(InputModeData);

			// 5. 마우스 커서를 화면에 표시
			bShowMouseCursor = true;
			
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ChatWidgetClass is NOT SET!"));
		}
	}
}

void ABaseBallPlayerController::MulticastShowNotice_Implementation(const FString& NoticeMessage)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, NoticeMessage);
	}
}

void ABaseBallPlayerController::ServerSendChat_Implementation(const FString& Message)
{
	// 1. GameMode 가져오기
	ABaseBallGameModeBase* GameMode = Cast<ABaseBallGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// 2. PlayerState 가져오기
	ABaseBallPlayerState* MyPlayerState = GetPlayerState<ABaseBallPlayerState>();

	if (GameMode && MyPlayerState)
	{
		// 3. GameMode를 통해 입력값(Message)이 유효한지 검사
		if (GameMode->IsValidGuess(Message))
		{
			// 4. 시도 횟수 증가 및 상태 업데이트
			MyPlayerState->IncreaseAttempt();
			FString Result = GameMode->CheckGuess(Message);
			MyPlayerState->SetLastResult(Result);

			// 5. 요구사항에 맞는 최종 메시지 형식 만들기
			FString FinalMessage = FString::Printf(TEXT("[%d/%d] %s : %s (%s)"),
				MyPlayerState->GetCurrentAttempt(),
				MyPlayerState->GetMaxAttempt(),
				*MyPlayerState->GetPlayerName(), // 플레이어 이름 가져오기
				*Message,
				*Result);

			// 6. 완성된 메시지를 모든 클라이언트에 전송
			MulticastReceiveChat(FinalMessage);

			// 7. 게임 종료 조건인지 확인
			GameMode->CheckGameEnd();
		}
		else
		{
			// 유효하지 않은 입력일 경우 보낸 사람에게만 알림 (선택 사항)
			ClientShowNotification(TEXT("3자리 중복되지 않는 숫자만 입력하세요."));
		}
	}
}

void ABaseBallPlayerController::MulticastReceiveChat_Implementation(const FString& Message)
{
	if (ChatWidget)
	{
		ChatWidget->AddChatMessage(Message); // 위젯의 업데이트 함수 호출
	}
}

void ABaseBallPlayerController::ClientShowNotification_Implementation(const FString& Message)
{
	if (GEngine)
	{
		// 화면 좌측 상단에 5초 동안 빨간색 디버그 메시지를 띄움
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	}
}