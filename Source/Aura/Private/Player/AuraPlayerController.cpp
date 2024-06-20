// Copyright DVNT Games


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>



AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// check will assert auracontext pointer can be found, and halt execution if not
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// init var for input mode to configure below
	FInputModeGameAndUI InputModeData;

	// makes sure not to lock mouse to the viewport
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	// prevents hiding the cursor once we click into the viewport
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	//super allows us to call the base class from the hierarchy of player controller
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
