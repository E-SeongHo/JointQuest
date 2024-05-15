// Fill out your copyright notice in the Description page of Project Settings.


#include "Server.h"


// Sets default values
AServer::AServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AServer::StartReceiver(
	const FString& SocketName,
	const FString& Address,
	const int32 Port
){
	return false;
}

void AServer::Listen(){}

void AServer::EndReceiver() {
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}


void AServer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Clear all sockets!
	//		makes sure repeat plays in Editor dont hold on to old sockets!
	EndReceiver();
}

// Called every frame
void AServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Listen();
}

