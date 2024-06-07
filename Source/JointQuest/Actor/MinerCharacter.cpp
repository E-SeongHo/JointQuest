

#include "MinerCharacter.h"

#include "../GameMode/DiggingSoilGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../PlayerController/MinerPlayerController.h"
#include "MovableActor.h"
#include "NetworkMessage.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ScoreComponent.h"
#include "../SoilGenerator.h"
#include "Components/SceneCaptureComponent2D.h"

AMinerCharacter::AMinerCharacter()
{
	// PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
		
	/*bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;*/

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; 	

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void AMinerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMovableActor::StaticClass(), MovableActors);

	PlayerController = Cast<AMinerPlayerController>(Controller);
	checkf(PlayerController, TEXT("Set PlayerController as MinerPlayerController"));
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	GameMode = Cast<ADiggingSoilGameMode>(UGameplayStatics::GetGameMode(this));
	checkf(GameMode, TEXT("Set Game Mode as DiggingSoilGameMode"));
}

void AMinerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FSM
	//UE_LOG(LogTemp, Display, TEXT("Current Tracking Status : %d"), PlayerController->GetCurrentStatus());

	switch(PlayerController->GetCurrentStatus())
	{
	case EJointTrackingStatus::Standing:
		break;
	case EJointTrackingStatus::Rising:
		break;
	case EJointTrackingStatus::Holding: // starts charging (play animation)
		if(!bIsCharging) BeginCharging();
		break;
	case EJointTrackingStatus::Falling: // quit charging (play animation)
		if(bIsCharging) EndCharging();
		break;
	}
}

void AMinerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMinerCharacter::Move);
	}
}

void AMinerCharacter::BeginCharging()
{
	UE_LOG(LogTemp, Display, TEXT("Begin Charging"));

	check(!bIsCharging);

	PlayAnimMontage(KneeUpAnimMontage);
	bIsCharging = true;
	UGameplayStatics::PlaySound2D(this, JumpingSound);

	GetWorld()->GetTimerManager().SetTimer(ChargingTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ChargedTime += GetWorld()->DeltaTimeSeconds;
		if(!bIsCharging)
		{
			GetWorldTimerManager().ClearTimer(ChargingTimerHandle);
		}
	}), GetWorld()->DeltaTimeSeconds, true);

}

void AMinerCharacter::EndCharging()
{
	check(bIsCharging);
	
	bIsCharging = false;
	UE_LOG(LogTemp, Display, TEXT("Charging Ended, Charged : %fs"), ChargedTime);

	const bool bHasSucceeded = ChargedTime > MinChargeDuration;
	PlayerController->GetScoreComponent()->RecordCurrentRep(bHasSucceeded);
	
	PlayAnimMontage(KneeDownAnimMontage);	
}

void AMinerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		const FVector ForwardDirection = FVector(1.0f, 0.0f, 0.0f);
		const FVector RightDirection = FVector(0.0f, 1.0f, 0.0f);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		SetActorRotation(GetVelocity().Rotation());
	}
}

void AMinerCharacter::StopJumping()
{
	Super::StopJumping();

}

void AMinerCharacter::TriggerDiggingNiagaraEffect(float Duration)
{
	checkf(DiggingEffect, TEXT("Digging Effect hasn't been bound"));

	FVector SpawnLocation;
	UNiagaraComponent* NiagaraComponent;

	// right side of the character
	SpawnLocation = GetActorLocation() + EffectOffset;
	NiagaraComponent =
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DiggingEffect, SpawnLocation);
	NiagaraComponent->SetFloatParameter(TEXT("Lifetime"), Duration);
	NiagaraComponent->SetAutoDestroy(true);
	
	// left side of the character
	SpawnLocation = GetActorLocation() + EffectOffset - FVector(0.0f, 2*EffectOffset.Y - 20.0f, 0.0f);
	NiagaraComponent =
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DiggingEffect, SpawnLocation);
	NiagaraComponent->SetFloatParameter(TEXT("Lifetime"), Duration);
	NiagaraComponent->SetAutoDestroy(true);
}

void AMinerCharacter::DigGround()
{
	const bool bHasSucceeded = ChargedTime > MinChargeDuration;
	//PlayerController->GetScoreComponent()->RecordCurrentRep(bHasSucceeded);

	const int32 Point = ChargedTime * 10.0f;
	const int32 Penalty = (PlayerController->GetFailedCnt() / 10) * 0.01f;
	PlayerController->GetScoreComponent()->AwardPoints(Point - Penalty);
	UE_LOG(LogTemp, Display, TEXT("Current Points : %d, FailedCnt Was : %d"), GameMode->GetScore(), PlayerController->GetFailedCnt());

	if(!bHasSucceeded)
	{
		StopAnimMontage();
		ChargedTime = 0.0f;
		return;
	}
	
	PlayerController->ClientStartCameraShake(DiggingShake);
	UE_LOG(LogTemp, Warning, TEXT("Shaking"));

	float MoveDuration = 1.5f; // hack
	
	for(AActor* MovableActor : MovableActors)
	{
		AMovableActor* MovingActor = Cast<AMovableActor>(MovableActor);
		MovingActor->LiftUp(500.0f * 4.0f, 50.0f, MoveDuration);
	}
	UGameplayStatics::PlaySound2D(this, DiggingSound);
	ChargedTime = 0.0f;

	TriggerDiggingNiagaraEffect(MoveDuration - 0.65f);
}

float AMinerCharacter::GetHoldingProgressPercent() const
{
	if(PlayerController->GetCurrentStatus() == EJointTrackingStatus::Rising ||
		PlayerController->GetCurrentStatus() == EJointTrackingStatus::Holding)
	{
		return ChargedTime / MinChargeDuration;
	}

	return 0.0f;
}


