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
		//�رգ�����
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

bool AActor_SocketTest::SocketCreate(FString IPStr, int32 port)
{
	// ���ַ���ת��ΪIPv4��ַ
	FIPv4Address::Parse(IPStr, ip);
	// ��ȡ����������ϵͳ�ĵ����׽�����ϵͳ,����һ�� FInternetAddr ����
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	// ����ip��ַ��һ�������ֽڹ���uint32
	addr->SetIp(ip.Value);
	// ���������ֽڹ���int�Ķ˿ں�
	addr->SetPort(port);
	// ����һ���׽���
	SocketClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	// ���׽������ӵ������ֽ������ַ������ɹ���Ϊtrue������Ϊfalse��
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

