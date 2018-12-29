// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_SocketServer.h"


// Sets default values
AActor_SocketServer::AActor_SocketServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//��ʼ��
	SocketServer = NULL;
	SocketClient = NULL;

}

// Called when the game starts or when spawned
void AActor_SocketServer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_SocketServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_SocketServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SocketServer)
	{
		//�رգ�����
		SocketServer->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketServer);
	}
	if (SocketClient)
	{
		//�رգ�����
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

// ����Socket
bool AActor_SocketServer::CreateSocketServer()
{
	SocketServer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP SocketServer"), false);

	if (!SocketServer) {
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("SocketServer Create failed!"));
		UE_LOG(LogTemp, Warning, TEXT("SocketServer Create failed!"));
		return false;
	}
	//SocketServer->SetNonBlocking(true); // �����׽�������Ϊ������ģʽ������ɹ���Ϊtrue������Ϊfalse��

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("SocketServer Create succ!"));
	UE_LOG(LogTemp, Warning, TEXT("SocketServer Create succ!"));
	return true;
}

// �� IP ���˿�
bool AActor_SocketServer::bindSocketServer(const FString & TheIP, const int32 ThePort)
{
	// ���ַ��� IPStr ת��ΪIPv4��ַ
	FIPv4Address::Parse(TheIP, ip);

	// ����һ�� addr ��� ip ��ַ�Ͷ˿�
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(ThePort);

	// �󶨵� IP ��ַ
	bool bBind = SocketServer->Bind(*addr);
	return bBind;
}

// ������������
bool AActor_SocketServer::listenSocketServer(const int32 MaxBacklog)
{
	bool bListen = SocketServer->Listen(MaxBacklog);
	return bListen;
}

// ���ܽ����ͻ�������
bool AActor_SocketServer::acceptSocketServer(const FString & TheIP, const int32 ThePort)
{
	FIPv4Address::Parse(TheIP, ip);

	// ����һ�� targetAddr ���Ŀ�� ip ��ַ�Ͷ˿�
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	targetAddr->SetIp(ip.Value);
	targetAddr->SetPort(ThePort);

	// ���ܹ��������
	SocketClient = SocketServer->Accept(*targetAddr, "acceptSocketClient");
	if (!SocketClient)
		return false;
	return true;
}

// ����Ϣ
bool AActor_SocketServer::sendSocketServer(const FString& sendMessage, bool bAddNull)
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	FString serialized = sendMessage;

	bool bsend;
	TCHAR *seriallizedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(seriallizedChar); // Ҫ���͵����ݴ�С��ĩβ��Ҫ��null�ַ�����ֵ +1��
	int32 sent = 0;

	if (bAddNull)
	{
		size++;
	}

	// ���ͻ�����; ע�⣬Ҫ�ÿͻ������socket
	bsend = SocketClient->SendTo((uint8*)TCHAR_TO_UTF8(seriallizedChar), size, sent, *targetAddr);

	if (bsend)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send Succ!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send Succ!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send failed!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send failed!"));
	}
	return bsend;
}

// ����Ϣ
void AActor_SocketServer::recvSocketServer()
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;//����һ��������
	uint32 Size;
	// �������ݰ�; ע�⣬Ҫ�ÿͻ������socket
	if (SocketClient->HasPendingData(Size))
	{
		uint8 *Recv = new uint8[Size];
		int32 BytesRead = 0;
		// ���������������������Ԫ�ء� ��Ԫ�ؽ�����ʼ����
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		// ע�⣬Ҫ�ÿͻ������socket
		SocketClient->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		if (ReceivedData.Num() > 0)
		{
			//��ӡ
			FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
			UE_LOG(LogTemp, Warning, TEXT("Server Recv log:   %s"), *ReceivedUE4String);

			// �ж��Ƿ�������Ե�ָ����ж�Ӧ�¼�����
			if (ReceivedUE4String.Equals("test"))
			{
				sendSocketServer("server auto send", true);
			}
		}
	}
}

// ��������תΪ String �ַ���
FString AActor_SocketServer::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

