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
		// �رգ�����
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

// ����Socket�����ӵ�������
bool AActor_SocketTest::SocketCreate(FString IPStr, int32 port)
{
	// ���ַ��� IPStr ת��ΪIPv4��ַ
	FIPv4Address::Parse(IPStr, ip);

	// ����һ�� addr ��� ip ��ַ�Ͷ˿�
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value); // ����ip��ַ
	addr->SetPort(port); // ���ö˿ں�

	// �����ͻ��� socket
	SocketClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP socket"), false);

	// ���׽������ӵ����� IP ��ַ������ɹ���Ϊtrue������Ϊfalse��
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

// ����Ϣ
void AActor_SocketTest::SocketSend(FString meesage, bool bAddNull)
{

	TCHAR *seriallizedChar = meesage.GetCharArray().GetData(); // Ҫ���͵Ļ�����
	int32 size = FCString::Strlen(seriallizedChar); // Ҫ���͵����ݴ�С��ĩβ��Ҫ��null�ַ�����ֵ +1��
	int32 sent = 0; // �ѷ��͵����ݴ�С

	if (bAddNull)
	{
		size++;
	}
	
	// ���ͻ�����
	if (SocketClient->Send((uint8*)TCHAR_TO_UTF8(seriallizedChar), size, sent))
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send Succ!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send Succ!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, TEXT("_____Send failed!"));
		UE_LOG(LogTemp, Warning, TEXT("_____Send failed!"));
	}
}

// ����Ϣ
void AActor_SocketTest::SocketReceive(bool & bReceive, FString & recvMessage)
{
	// ��ʼ������
	recvMessage = "";
	bReceive = false;

	// ����ͻ��˲������򷵻�
	if (!SocketClient)
	{
		return;
	}

	uint32 size;
	TArray<uint8> ReceiveData;
	uint8 element = 0;
	// �������ݰ�
	while (SocketClient->HasPendingData(size)) // ��ѯ�׽�����ȷ���������Ƿ���δ������
	{
		ReceiveData.Init(element, FMath::Min(size, 65507u));
		int32 read = 0;

		SocketClient->Recv(ReceiveData.GetData(), ReceiveData.Num(), read);
	}

	// �������Ϊ���򷵻�
	if (ReceiveData.Num() <= 0)
	{
		return;
	}

	FString log = "Total Data read! num: " + FString::FromInt(ReceiveData.Num());
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, log);
	UE_LOG(LogTemp, Warning, TEXT("Recv log:   %s"), *log);

	// ���յ�����תΪ String �ַ���
	const FString ReceivedUE4String = StringFromBinaryArray(ReceiveData);

	log = "Server:" + ReceivedUE4String;
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, log);
	UE_LOG(LogTemp, Warning, TEXT("*** %s"), *log);

	recvMessage = ReceivedUE4String;

	bReceive = true;
}

// ��������תΪ String �ַ���
FString AActor_SocketTest::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

