
#include "SoilGeneratorActor.h"
#include "MovableActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"


ASoilGeneratorActor::ASoilGeneratorActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ASoilGeneratorActor::BeginPlay()
{	
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Soil Generator Init"));

	const float HalfWidth = 1000.0f;
	const float HalfDepth = 250.0f;
	
	FVector CharacterLocation = UGameplayStatics::GetPlayerCharacter(this, 0)->GetActorLocation();
	
	AActor* Stage = GetWorld()->SpawnActor<AActor>(CharacterStage,
			CharacterLocation - FVector(0.0f, -15.0f, 2 * HalfWidth + 50.0f), 
			FRotator::ZeroRotator);

	RefLocation = Stage->GetActorLocation();
	
	// hack : for test
	LeftGenPos = {RefLocation.X-5.9f, RefLocation.Y + HalfWidth + 60.0f};
	RightGenPos = {RefLocation.X-5.9f ,RefLocation.Y - HalfWidth - 60.0f};
	BackGenPos = {RefLocation.X - HalfDepth * 2 - 75.0f,0.0f};
	
	GenerateSoils();
}

void ASoilGeneratorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(TailPtr);
	//UE_LOG(LogTemp, Display, TEXT("Tail Z : %s"), *TailPtr->GetActorLocation().ToString());
	
	if(TailPtr->GetActorLocation().Z > RearrangeHeight)
	{
		RearrangeSoils();
	}
}

void ASoilGeneratorActor::GenerateSoils()
{
	AMovableActor** PeekPtr = LeftSoilsQueue.Peek();
	AMovableActor* PeekElem = PeekPtr ? *PeekPtr : nullptr;
	
	//float CurrentHeight = PeekElem ? PeekElem->GetActorLocation().Z : 3427.0f;
	float CurrentHeight = PeekElem ? PeekElem->GetActorLocation().Z : RefLocation.Z + 2100.0f;

	FVector LeftSideSpawnLocation(LeftGenPos.X, LeftGenPos.Y, 0.0f);
	FVector RightSideSpawnLocation(RightGenPos.X, RightGenPos.Y, 0.0f);
	FVector BackSideSpawnLocation(BackGenPos.X, BackGenPos.Y, 0.0f);
	
	for(int32 i = 0; i < GenAmount; i++)
	{
		UE_LOG(LogTemp, Display, TEXT("Soil Generator Gens %d"), i);

		float SpawnHeight = CurrentHeight - (BlockHeight * i);
		LeftSideSpawnLocation.Z = SpawnHeight;
		RightSideSpawnLocation.Z = SpawnHeight;
		BackSideSpawnLocation.Z = SpawnHeight;
		
		AActor* LeftGen = GetWorld()->SpawnActor<AMovableActor>(LeftBlockTemplate,
			LeftSideSpawnLocation,
			FRotator::ZeroRotator);
		UE_LOG(LogTemp, Display, TEXT("%s"), *LeftGen->GetActorLocation().ToString());
		
		AActor* RightGen = GetWorld()->SpawnActor<AMovableActor>(RightBlockTemplate,
			RightSideSpawnLocation,
			FRotator::ZeroRotator);

		AActor* BackGen = GetWorld()->SpawnActor<AMovableActor>(BackBlockTemplate,
			BackSideSpawnLocation,
			FRotator::ZeroRotator);

		LeftSoilsQueue.Enqueue(Cast<AMovableActor>(LeftGen));
		RightSoilsQueue.Enqueue(Cast<AMovableActor>(RightGen));
		BackSoilsQueue.Enqueue(Cast<AMovableActor>(BackGen));

		GeneratedBlocks++;

		if(i == GenAmount - 1)
		{
			TailPtr = Cast<AMovableActor>(BackGen);
		}
	}
}

void ASoilGeneratorActor::RearrangeSoils()
{
	UE_LOG(LogTemp, Display, TEXT("Rearrange Soils"));

	int FixNums = 4;
	float CurrentHeight = TailPtr->GetActorLocation().Z;

	for(int32 i = 0; i < GenAmount - FixNums; i++)
	{
		float TargetHeight = CurrentHeight - ((i+1) * BlockHeight);

		FVector LeftSideSpawnLocation(LeftGenPos.X, LeftGenPos.Y, TargetHeight);
		FVector RightSideSpawnLocation(RightGenPos.X, RightGenPos.Y, TargetHeight);
		FVector BackSideSpawnLocation(BackGenPos.X, BackGenPos.Y, TargetHeight);
		
		AMovableActor* Temp;

		LeftSoilsQueue.Dequeue(Temp);
		Temp->SetActorLocation(LeftSideSpawnLocation);
		LeftSoilsQueue.Enqueue(Temp);

		RightSoilsQueue.Dequeue(Temp);
		Temp->SetActorLocation(RightSideSpawnLocation);
		RightSoilsQueue.Enqueue(Temp);

		BackSoilsQueue.Dequeue(Temp);
		Temp->SetActorLocation(BackSideSpawnLocation);
		BackSoilsQueue.Enqueue(Temp);

		TailPtr = Temp;
	}
}
