// Copyright DVNT Games


#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	// Create the subobject skeletal mesh for the weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	// Attach the weapon to a socket on the skeletal mesh
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

	// Set no collisions for the weapon
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

