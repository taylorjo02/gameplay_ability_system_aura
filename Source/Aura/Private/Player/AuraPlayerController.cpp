// Copyright DVNT Games


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include <Interaction/EnemyInterface.h>



AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	* Line Trace from cursor. There are several scenarios:
	* 
	* A. Last Actor is null && This Actor is null
	*	- Do nothing
	* 
	* B. Last Actor is null && This Actor is valid
	*	- Since last actor is null, this is the first frame our cursor has hit the object. Call HighlightActor() to highlight the EnemyInterface object
	* 
	* C. Last Actor is valid && This Actor is null
	*	- No longer hovering over enemy, so we want to unhighlight the actor by calling UnHighlightActor() on Last Actor
	* 
	* D. Both actors are valid, but Last Actor != to This Actor
	*	- Hovering over one enemy, but now hovering over a different enemy so Unhighlight Last Actor & Highlight This Actor
	* 
	* E. Both actors are valid & are the same actor
	*	- Enemy that was highlighted is still highlighted, so in this case we do nothing
	*/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B - This Actor is valid and not null
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, Do Nothing
		}
	}
	else // Last Actor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C - UnHighlight Actor
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else // both actors are equal
			{
				// Case E - Do Nothing
			}
		}
	}


}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// check will assert auracontext pointer can be found, and halt execution if not
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

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
