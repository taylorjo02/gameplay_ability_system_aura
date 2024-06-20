// Copyright DVNT Games


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"



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