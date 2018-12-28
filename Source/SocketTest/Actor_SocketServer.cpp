// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_SocketServer.h"


// Sets default values
AActor_SocketServer::AActor_SocketServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//初始化
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
		//关闭，销毁
		SocketServer->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketServer);
	}
	if (SocketClient)
	{
		//关闭，销毁
		SocketClient->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SocketClient);
	}
}

// 创建Socket
bool AActor_SocketServer::CreateSocketServer()
{
	SocketServer = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP SocketServer"), false);

	if (!SocketServer) {
		return false;
	}
	SocketServer->SetNonBlocking(false); // 将此套接字设置为非阻塞模式。如果成功则为true，否则为false。
	return true;
}

// 绑定 IP 、端口
bool AActor_SocketServer::bindSocketServer(const FString & TheIP, const int32 ThePort)
{
	// 将字符串 IPStr 转换为IPv4地址
	FIPv4Address::Parse(TheIP, ip);

	// 创建一个 addr 存放 ip 地址和端口
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(ThePort);

	// 绑定到 IP 地址
	bool bBind = SocketServer->Bind(*addr);
	return bBind;
}

// 侦听传入连接
bool AActor_SocketServer::listenSocketServer(const int32 MaxBacklog)
{
	bool bListen = SocketServer->Listen(MaxBacklog);
	return bListen;
}

// 接受建立客户端连接
bool AActor_SocketServer::acceptSocketServer(const FString & TheIP, const int32 ThePort)
{
	FIPv4Address::Parse(TheIP, ip);

	// 创建一个 targetAddr 存放目标 ip 地址和端口
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	targetAddr->SetIp(ip.Value);
	targetAddr->SetPort(ThePort);

	// 接受挂起的连接
	SocketClient = SocketServer->Accept(*targetAddr, "acceptSocketClient");
	if (!SocketClient)
		return false;
	return true;
}

// 发消息
bool AActor_SocketServer::sendSocketServer(const FString& sendMessage)
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	FString serialized = sendMessage;

	bool bsend;
	TCHAR *seriallizedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(seriallizedChar) + 1; // 要发送的数据大小（末尾若要加null字符，则值 +1）
	int32 sent = 0;
	// 发送缓冲区; 注意，要用客户端这个socket
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

// 收消息
void AActor_SocketServer::recvSocketServer()
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;//定义一个接收器
	uint32 Size;
	// 接收数据包; 注意，要用客户端这个socket
	if (SocketClient->HasPendingData(Size))
	{
		uint8 *Recv = new uint8[Size];
		int32 BytesRead = 0;
		// 将数组调整到给定数量的元素。 新元素将被初始化。
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		// 注意，要用客户端这个socket
		SocketClient->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		if (ReceivedData.Num() > 0)
		{
			//打印
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));
			FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
			//判断是否发送了相对的指令，进行对应事件调用
			if (ReceivedUE4String.Equals("test"))
			{
				sendSocketServer("server auto send ");
			}
		}
	}
}

// 将二进制转为 String 字符串
FString AActor_SocketServer::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

