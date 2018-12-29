// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Runtime/Networking/Public/Networking.h"

#include "Actor_SocketServer.generated.h"

UCLASS()
class SOCKETTEST_API AActor_SocketServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_SocketServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** ÿ����һ���ؿ��Ƴ���actorʱ���� */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ����Socket
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool CreateSocketServer();

	// �� IP ���˿�
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool bindSocketServer(const FString& TheIP, const int32 ThePort);

	// ������������
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool listenSocketServer(const int32 MaxBacklog);

	// ���ܽ����ͻ�������
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool acceptSocketServer(const FString& TheIP, const int32 ThePort);

	// ����Ϣ
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool sendSocketServer(const FString& sendMessage, bool bAddNull);

	// ����Ϣ
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		void recvSocketServer();

	// ��������תΪ String �ַ���
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	FSocket* SocketServer;
	FSocket* SocketClient;
	FIPv4Address ip;
	
	
};
