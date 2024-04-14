

#include "MinerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MovableActor.h"
#include "NetworkMessage.h"
#include "Kismet/GameplayStatics.h"
#include "SoilGenerator.h"

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

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void AMinerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	check(!bIsCharging);

	PlayAnimMontage(KneeUpAnimMontage);
	bIsCharging = true;

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
	UE_LOG(LogTemp, Display, TEXT("Start Digging, Charged : %f"), ChargedTime);

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

void AMinerCharacter::DigGround()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	PC->ClientStartCameraShake(DiggingShake);

	for(AActor* MovableActor : MovableActors)
	{
		AMovableActor* MovingActor = Cast<AMovableActor>(MovableActor);
		MovingActor->LiftUp(500.0f, 50.0f);
	}

	ChargedTime = 0.0f;
}


