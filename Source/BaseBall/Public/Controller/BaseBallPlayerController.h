// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "BaseBallPlayerController.generated.h"

class UBaseBallUserWidget;
class UTextBlock;
class UBaseBallChatWidget;
/**
 * 
 */
UCLASS()
class BASEBALL_API ABaseBallPlayerController : public APlayerController
{
	GENERATED_BODY()

	
public:
	virtual void BeginPlay() override;
	
	// 클라이언트에서 호출 -> 서버로 전송
	UFUNCTION(Server, Reliable)
	void ServerSendChat(const FString& Message);

	// 서버에서 모든 클라이언트로 브로드캐스트
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReceiveChat(const FString& Message);

	UFUNCTION(Client, Reliable)
	void ClientShowNotification(const FString& Message);

	// 클라이언트 UI 갱신용
	// void AddChatToUI(const FString& Message);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ChatWidgetClass;  // 위젯 블루프린트 클래스 참조

	UPROPERTY()
	UBaseBallUserWidget* ChatWidget; // 실제 인스턴스

	UFUNCTION(NetMulticast, Reliable)
	void MulticastShowNotice(const FString& NoticeMessage);
};
