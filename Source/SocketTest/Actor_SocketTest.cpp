// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_SocketTest.h"


// Sets default values
AActor_SocketTest::AActor_SocketTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SocketClient = NULL;

}

// Called when the game starts or when spawned
void AActor_SocketTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_SocketTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_SocketTest::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SocketClient)
	{
		//关闭，销毁
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

bool AActor_SocketTest::SocketCreate(FString IPStr, int32 port)
{
	// 将字符串转换为IPv4地址
	FIPv4Address::Parse(IPStr, ip);
	// 获取给定命名子系统的单例套接字子系统,创建一个 FInternetAddr 代表
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	// 设置ip地址从一个主机字节规则uint32
	addr->SetIp(ip.Value);
	// 设置主机字节规则int的端口号
	addr->SetPort(port);
	// 创建一个套接字
	SocketClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	// 将套接字连接到网络字节有序地址。如果成功则为true，否则为false。
	if (SocketClient->Connect(*addr))
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("Connect Succ!"));
		UE_LOG(LogTemp, Warning, TEXT("Connect Succ!"));
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("Connect failed!"));
		UE_LOG(LogTemp, Warning, TEXT("Connect failed!"));
		return false;
	}
	
}

void AActor_SocketTest::SocketSend(FString meesage)
{

}

