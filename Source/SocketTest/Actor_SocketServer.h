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

	/** 每当从一个关卡移除此actor时调用 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 创建Socket
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool CreateSocketServer();

	// 绑定 IP 、端口
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool bindSocketServer(const FString& TheIP, const int32 ThePort);

	// 侦听传入连接
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool listenSocketServer(const int32 MaxBacklog);

	// 接受建立客户端连接
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool acceptSocketServer(const FString& TheIP, const int32 ThePort);

	// 发消息
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		bool sendSocketServer(const FString& sendMessage, bool bAddNull);

	// 收消息
	UFUNCTION(BlueprintCallable, Category = "CGA Socket")
		void recvSocketServer();

	// 将二进制转为 String 字符串
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	FSocket* SocketServer;
	FSocket* SocketClient;
	FIPv4Address ip;
	
	
};
