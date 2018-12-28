// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Runtime/Networking/Public/Networking.h"

#include "Actor_SocketTest.generated.h"

UCLASS()
class SOCKETTEST_API AActor_SocketTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_SocketTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** ÿ����һ���ؿ��Ƴ���actorʱ���� */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// ����Socket�����ӵ�������
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool SocketCreate(FString IPStr, int32 port);

	// ����Ϣ
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		void SocketSend(FString meesage);

	// ����Ϣ
	UFUNCTION(BlueprintPure, Category = "CGA Socket")
		void SocketReceive(bool& bReceive, FString& recvMessage);

	//UFUNCTION(BlueprintCallable, Category = "CGA Socket")
	//	bool ThreadEnd();

	// ��������תΪ String �ַ���
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);

public:
	FSocket *SocketClient;
	FIPv4Address ip;
	FRunnableThread* m_RecvThread;
};
