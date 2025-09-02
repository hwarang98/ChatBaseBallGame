// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseBallUserWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Controller/BaseBallPlayerController.h"

void UBaseBallUserWidget::AddChatMessage(const FString& Message)
{
	// 스크롤 박스가 유효한지 확인
	if (ChatScrollBox)
	{
		// 새로 표시할 TextBlock 위젯을 동적으로 생성
		UTextBlock* NewMessageBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
		if (NewMessageBlock)
		{
			NewMessageBlock->SetText(FText::FromString(Message));
			NewMessageBlock->SetFont(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 16)); // 폰트 설정 (예시)
			NewMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));

			// 스크롤 박스에 자식으로 추가하고 맨 아래로 스크롤
			ChatScrollBox->AddChild(NewMessageBlock);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void UBaseBallUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// SendButton이 유효한지 확인하고 OnClicked 이벤트에 함수를 연결(바인딩)합니다.
	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UBaseBallUserWidget::OnSendButtonClicked);
	}
}

void UBaseBallUserWidget::OnSendButtonClicked()
{
	// 1. 입력창과 PlayerController가 유효한지 확인
	if (InputTextBox && GetOwningPlayer())
	{
		// 2. 입력된 텍스트를 가져옵
		FString Message = InputTextBox->GetText().ToString();

		// 3. 텍스트가 비어있지 않다면 서버로 전송
		if (!Message.IsEmpty())
		{
			ABaseBallPlayerController* PC = Cast<ABaseBallPlayerController>(GetOwningPlayer());
			if (PC)
			{
				PC->ServerSendChat(Message);
			}

			// 4. 메시지를 보낸 후 입력창을 비움
			InputTextBox->SetText(FText::GetEmpty());
		}
	}
}
