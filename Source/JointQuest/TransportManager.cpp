// Fill out your copyright notice in the Description page of Project Settings.


#include "TransportManager.h"

// Sets default values
ATransportManager::ATransportManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransportManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransportManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FString ATransportManager::BytestreamToString(TArray<uint8> bytes) {
	/*
	uint8* arr = new uint8[bytes.Num()];
	for (int i = 0; i < bytes.Num(); i++) {
		arr[i] = bytes[i];
	}
	return BytesToString(arr, bytes.Num());
	*/
	return FString(bytes.Num(), UTF8_TO_TCHAR(bytes.GetData()));
}


TArray<uint8> ATransportManager::StringToBytestream(FString text) {
	TArray<uint8> res;
	const int32 len = FTCHARToUTF8_Convert::ConvertedLength(*text, text.Len());
	res.SetNumUninitialized(len);
	FTCHARToUTF8_Convert::Convert((UTF8CHAR*)res.GetData(), res.Num(), *text, text.Len());

	return res;
}