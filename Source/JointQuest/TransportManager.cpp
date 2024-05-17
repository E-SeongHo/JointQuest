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

TSharedPtr<FJsonObject> ATransportManager::ParseJson(FString text) {
	TSharedPtr<FJsonObject> res;
	auto Reader = TJsonReaderFactory<>::Create(text);
	if (FJsonSerializer::Deserialize(Reader, res)) {
		return res;
	}
	return nullptr;
}

FString ATransportManager::GetEncodedImage(TSharedPtr<FJsonObject> json) {
	FString res;
	if (json->TryGetStringField("image", res)) {
		return res;
	}

	return TEXT("");
}

UTexture2D* ATransportManager::GetEncodedImage(FString text) {
	return DecodeImage(GetEncodedImage(ParseJson(text)));
}

UTexture2D* ATransportManager::DecodeImage(FString text) {
	TArray<uint8> buffer;
	FString left, right;
	
	text.Split(TEXT(","), &left, &right);
	
	if (right == "") {
		return nullptr;
	}

	bool isDecode = FBase64::Decode(right, buffer);
	if (isDecode) {
		return CreateTextureFromBits(buffer);
	}
	else {
		return nullptr;
	}
}

UTexture2D* ATransportManager::CreateTextureFromBits(TArray<uint8> data) {
	UTexture2D* res = FImageUtils::ImportBufferAsTexture2D(data);
	
	res->MipGenSettings = TMGS_NoMipmaps;
	res->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	res->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	res->SRGB = false;
	res->Filter = TextureFilter::TF_Nearest;
	res->UpdateResource();

	return res;
}