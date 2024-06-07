// Fill out your copyright notice in the Description page of Project Settings.


#include "TransportManager.h"


float ATransportManager::JointAngle = -1.0f;
float ATransportManager::SubAngle1 = -1.0f;
float ATransportManager::SubAngle2 = -1.0f;

FString ATransportManager::BodyData = TEXT("null");
UTexture2D* ATransportManager::CurrentWebcamDisplay = nullptr;

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

UTexture2D* ATransportManager::GetCurrentWebcamDisplay()
{
	return CurrentWebcamDisplay;
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

UTexture2D* ATransportManager::DecodeImage(FString text) {

	TArray<uint8> buffer;

	/*
	FString left, right;
	text.Split(TEXT(","), &left, &right);

	if (right == "") {

		UE_LOG(LogTemp, Warning, TEXT("Decoding failure"));
		return nullptr;
	}

	bool isDecode = FBase64::Decode(right, buffer);
	*/
	bool isDecode = FBase64::Decode(text, buffer);
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

	CurrentWebcamDisplay = res;
	return res;
}

float ATransportManager::GetJointAngle() {
	return JointAngle;
}

void ATransportManager::SetJointAngle(float value){
	if (value < 0.1) { return; }

	JointAngle = value;
}
float ATransportManager::GetSubAngle1() {
	return SubAngle1;
}

void ATransportManager::SetSubAngle1(float value) {
	if (value < 0.1) { return; }
	SubAngle1 = value;
}
float ATransportManager::GetSubAngle2() {
	return SubAngle2;
}

void ATransportManager::SetSubAngle2(float value) {
	if (value < 0.1) { return; }
	SubAngle2 = value;
}

bool ATransportManager::SaveImage(UTexture2D* texture, const FString& path)
{
	TArray<FColor> colors;

	const FColor* FormatedImageData = static_cast<const FColor*>(texture->PlatformData->Mips[0].BulkData.LockReadOnly());

	for (int32 Y = 0; Y < texture->GetSizeY(); Y++)
	{
		for (int32 X = 0; X < texture->GetSizeX(); X++)
		{
			FColor PixelColor = FormatedImageData[Y * texture->GetSizeX() + X];
			colors.Add(PixelColor);
		}
	}

	texture->PlatformData->Mips[0].BulkData.Unlock();

	TArray<uint8> compressedBitmap;
	FImageUtils::CompressImageArray(texture->GetSizeX(), texture->GetSizeY(), colors, compressedBitmap);

	bool isSavedImage = FFileHelper::SaveArrayToFile(compressedBitmap, *path);

	return isSavedImage;
}

void ATransportManager::SetBodyData(FString data) {
	BodyData = data;
}

FString ATransportManager::GetBodyData(){
	return BodyData;
}
