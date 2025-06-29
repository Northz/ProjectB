// Fill out your copyright notice in the Description page of Project Settings.


#include "NovMovementComponent.h"
#include "GameFramework/Character.h"

bool UNovMovementComponent::FSavedMove_Nov::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Nov* NewNovMove = static_cast<FSavedMove_Nov*>(NewMove.Get());

	if (bSavedWantsToWalk != NewNovMove->bSavedWantsToWalk)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UNovMovementComponent::FSavedMove_Nov::Clear()
{
	FSavedMove_Character::Clear();
	bSavedWantsToWalk = 0;
}

uint8 UNovMovementComponent::FSavedMove_Nov::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedWantsToWalk) Result |= FLAG_Custom_0;

	return Result;
}

void UNovMovementComponent::FSavedMove_Nov::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UNovMovementComponent* CharacterMovement = Cast<UNovMovementComponent>(Character->GetCharacterMovement());

	bSavedWantsToWalk = CharacterMovement->bWantsToWalk;
}

void UNovMovementComponent::FSavedMove_Nov::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UNovMovementComponent* CharacterMovement = Cast<UNovMovementComponent>(Character->GetCharacterMovement());

	CharacterMovement->bWantsToWalk = bSavedWantsToWalk;
}

UNovMovementComponent::FNetworkPredictionData_Client_Nov::FNetworkPredictionData_Client_Nov(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
	// The above should be shorthand for:
	// FNetworkPredictionData_Client_Character(ClientMovement);
}

FSavedMovePtr UNovMovementComponent::FNetworkPredictionData_Client_Nov::AllocateNewMove()
{
	// Heap allocation with new because our helpers
	// are not inheriting from UObject
	return FSavedMovePtr(new FSavedMove_Nov());
}

FNetworkPredictionData_Client* UNovMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

		if (ClientPredictionData == nullptr)
		{
			UNovMovementComponent* MutableThis = const_cast<UNovMovementComponent*>(this);

			MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Nov(*this);
			MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
			MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
		}
	return ClientPredictionData;
}

void UNovMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToWalk = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UNovMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (bWantsToWalk)
		{
			MaxWalkSpeed = MaxWalkingSpeed;
		}
		else
		{
			MaxWalkSpeed = MaxRunningSpeed;
		}
	}
}

void UNovMovementComponent::ToggleWalk()
{
	bWantsToWalk = !bWantsToWalk;
}
