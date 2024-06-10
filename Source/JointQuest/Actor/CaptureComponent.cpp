// JNU-JointQuest


#include "CaptureComponent.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "JointQuest/TransportManager.h"
#include "JointQuest/GameInstance/JointQuestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../TransportManager.h"
#include "JointQuest/PlayerController/MinerPlayerController.h"

UCaptureComponent::UCaptureComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCaptureComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UJointQuestGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(GameInstance);

	FString Path = FPaths::ProjectContentDir() / TEXT("Capture");
	if (IFileManager::Get().DirectoryExists(*Path))
	{
		IFileManager::Get().DeleteDirectory(*Path, false, true);
	}
}

bool UCaptureComponent::SaveTextureToBinary(UTexture2D* Texture, const FString& FilePath)
{
	if (!Texture) return false;

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

	TArray<uint8> TextureData;
	TextureData.SetNumUninitialized(Mip.BulkData.GetBulkDataSize());
	FMemory::Memcpy(TextureData.GetData(), Data, Mip.BulkData.GetBulkDataSize());

	Mip.BulkData.Unlock();

	FArrayWriter Writer;
	Writer << TextureData;

	return FFileHelper::SaveArrayToFile(Writer, *FilePath);
}

bool UCaptureComponent::SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath)
{
	if (!Texture) return false;

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

	TArray<FColor> OutPixels;
	OutPixels.SetNumUninitialized(Mip.SizeX * Mip.SizeY);

	FMemory::Memcpy(OutPixels.GetData(), Data, Mip.BulkData.GetBulkDataSize());

	Mip.BulkData.Unlock();

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(OutPixels.GetData(), OutPixels.GetAllocatedSize(), Mip.SizeX, Mip.SizeY, ERGBFormat::BGRA, 8))
	{
		const TArray64<uint8>& PNGData = ImageWrapper->GetCompressed(100);
		
		return FFileHelper::SaveArrayToFile(PNGData, *FilePath);
	}

	return false;
}

UTexture2D* UCaptureComponent::LoadPNGToTexture(const FString& FilePath)
{
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
		return nullptr;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (!ImageWrapper.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create image wrapper for file: %s"), *FilePath);
		return nullptr;
	}

	if (!ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to decompress image: %s"), *FilePath);
		return nullptr;
	}

	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get raw image data: %s"), *FilePath);
		return nullptr;
	}

	UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
	if (!Texture)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create texture: %s"), *FilePath);
		return nullptr;
	}

	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

	Texture->UpdateResource();

	return Texture;
}

void UCaptureComponent::BeginCapture()
{
	if(bIsCapturing) return;

	bIsCapturing = true;
	CapturingIndex = GameInstance->GetRecords().Num();
	FrameIndex = 0;
	TimeElapsedFromLastCapture = 0.0f;

	CaptureDirectory = FPaths::ProjectContentDir() / TEXT("Capture") / FString::Printf(TEXT("Rep%d"), CapturingIndex);
	IFileManager::Get().MakeDirectory(*CaptureDirectory, true);
}

void UCaptureComponent::EndCapture()
{
	check(bIsCapturing);

	bIsCapturing = false;
}

void UCaptureComponent::GameEnd()
{
	// Loads POTG Data
	const int32 POTGIndex = GameInstance->GetBestRepIndex();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString Directory = FPaths::ProjectContentDir() / TEXT("Capture") / FString::Printf(TEXT("Rep%d"), POTGIndex);
	int32 Frame = 0;
	FString Path = Directory / FString::Printf(TEXT("_%d.png"), Frame++);

	while(PlatformFile.FileExists(*Path))
	{
		POTGData.Add(LoadPNGToTexture(Path));
		Path = Directory / FString::Printf(TEXT("_%d.png"), Frame++);
	}

	UE_LOG(LogTemp, Display, TEXT("Rep%d : %d"), POTGIndex, POTGData.Num());
}

UTexture2D* UCaptureComponent::StreamPOTG()
{
	// Ensure GameEnd is called so POTGData is Loaded.
	UTexture2D* TextureToStream = POTGData[StreamIdx];
	StreamIdx = (StreamIdx + 1) % POTGData.Num();

	return TextureToStream;
}

void UCaptureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// target frame rate : 30fps
	if(bIsCapturing)
	{
		TimeElapsedFromLastCapture += DeltaTime;

		if (TimeElapsedFromLastCapture >= TargetFrameInterval)
		{
			TimeElapsedFromLastCapture = 0.0f;
			const FString FilePath =  CaptureDirectory / FString::Printf(TEXT("_%d.png"), FrameIndex++);
			SaveTextureToPNG(Cast<AMinerPlayerController>(GetOwner())->GetTransportManager()->GetCurrentWebcamDisplay(), FilePath);
		}
	}
}