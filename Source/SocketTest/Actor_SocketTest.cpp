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
		// 关闭，销毁
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

// 创建Socket并连接到服务器
bool AActor_SocketTest::SocketCreate(FString IPStr, int32 port)
{
	// 将字符串 IPStr 转换为IPv4地址
	FIPv4Address::Parse(IPStr, ip);

	// 创建一个 addr 存放 ip 地址和端口
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value); // 设置ip地址
	addr->SetPort(port); // 设置端口号

	// 创建客户端 socket
	SocketClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP socket"), false);

	// 将套接字连接到网络 IP 地址。如果成功则为true，否则为false。
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

// 发消息
void AActor_SocketTest::SocketSend(FString meesage, bool bAddNull)
{

	TCHAR *seriallizedChar = meesage.GetCharArray().GetData(); // 要发送的缓冲区
	int32 size = FCString::Strlen(seriallizedChar); // 要发送的数据大小（末尾若要加null字符，则值 +1）
	int32 sent = 0; // 已发送的数据大小

	if (bAddNull)
	{
		size++;
	}
	
	// 发送缓冲区
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

// 收消息
void AActor_SocketTest::SocketReceive(bool & bReceive, FString & recvMessage)
{
	// 初始化变量
	recvMessage = "";
	bReceive = false;

	// 如果客户端不存在则返回
	if (!SocketClient)
	{
		return;
	}

	uint32 size;
	TArray<uint8> ReceiveData;
	uint8 element = 0;
	// 接收数据包
	while (SocketClient->HasPendingData(size)) // 查询套接字以确定队列中是否有未决数据
	{
		ReceiveData.Init(element, FMath::Min(size, 65507u));
		int32 read = 0;

		SocketClient->Recv(ReceiveData.GetData(), ReceiveData.Num(), read);
	}

	// 如果数据为空则返回
	if (ReceiveData.Num() <= 0)
	{
		return;
	}

	FString log = "Total Data read! num: " + FString::FromInt(ReceiveData.Num());
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, log);
	UE_LOG(LogTemp, Warning, TEXT("Recv log:   %s"), *log);

	// 接收的数据转为 String 字符串
	const FString ReceivedUE4String = StringFromBinaryArray(ReceiveData);

	log = "Server:" + ReceivedUE4String;
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, log);
	UE_LOG(LogTemp, Warning, TEXT("*** %s"), *log);

	recvMessage = ReceivedUE4String;

	bReceive = true;
}

// 将二进制转为 String 字符串
FString AActor_SocketTest::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

