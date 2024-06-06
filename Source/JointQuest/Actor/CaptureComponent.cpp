// JNU-JointQuest


#include "CaptureComponent.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "JointQuest/TransportManager.h"
#include "JointQuest/GameInstance/JointQuestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../TransportManager.h"

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

	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

	TArray<uint8> TextureData;
	TextureData.SetNumUninitialized(Mip.BulkData.GetBulkDataSize());
	FMemory::Memcpy(TextureData.GetData(), Data, Mip.BulkData.GetBulkDataSize());

	Mip.BulkData.Unlock();

	FArrayWriter Writer;
	Writer << TextureData;

	return FFileHelper::SaveArrayToFile(Writer, *(FilePath / TEXT(".bin")));
}

bool UCaptureComponent::SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath)
{
	if (!Texture) return false;

	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
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
		
		return FFileHelper::SaveArrayToFile(PNGData, *(FilePath / TEXT(".png")));
	}

	return false;
}

void UCaptureComponent::BeginCapture()
{
	//check(!bIsCapturing);

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

void UCaptureComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// target frame : 30fps
	if(bIsCapturing)
	{
		TimeElapsedFromLastCapture += DeltaTime;

		if (TimeElapsedFromLastCapture >= TargetFrameInterval)
		{
			TimeElapsedFromLastCapture = 0.0f;
			const FString FilePath =  CaptureDirectory / FString::Printf(TEXT("_%d"), FrameIndex++);
			SaveTextureToBinary(ATransportManager::GetCurrentWebcamDisplay(), FilePath);
		}
	}
}

