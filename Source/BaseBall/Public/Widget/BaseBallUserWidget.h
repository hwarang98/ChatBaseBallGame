// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "BaseBallUserWidget.generated.h"

class UButton;
class UScrollBox;
/**
 * 
 */
UCLASS()
class BASEBALL_API UBaseBallUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PlayerController가 호출할 UI 업데이트 전용 함수
	void AddChatMessage(const FString& Message);

protected:
	virtual void NativeConstruct() override;

#pragma region UI
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox; // WBP에서 만든 스크롤 박스의 이름과 일치해야함

	UPROPERTY(meta = (BindWidget))
	UEditableText* InputTextBox;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SendButton; // 버튼 변수
#pragma endregion 

private:
	UFUNCTION()
	void OnSendButtonClicked();
};
