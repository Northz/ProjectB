// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NovMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTB_API UNovMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	// Helper class, snapshots all state data in this movement component
	// in order to reproduce a move for a given frame
	class FSavedMove_Nov : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		// Saved bWantsToWalk
		uint8 bSavedWantsToWalk : 1;

		// Checks current move and new move and tries to combine them
		// i.e. when the saved data for two moves is the same
		// saves bandwidth
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

		// Resets the save move object to empty
		virtual void Clear() override;

		// Returns a byte (8-bit) containing special movement data
		// to extend we must use assigned custom flags
		// use for only essential data that is sent every single frame
		// i.e. don't use for dashing, which should instead be handled by 
		// an RPC (remote procedure call) to the server
		virtual uint8 GetCompressedFlags() const override;

		// Captures the state data of the CMC
		virtual void SetMoveFor(
			ACharacter* Character, 
			float InDeltaTime, 
			FVector const& NewAccel,
			FNetworkPredictionData_Client_Character& ClientData) override;

		// Takes the data from the set move and applies it to the current state
		// basically the reverse of SetMoveFor.
		virtual void PrepMoveFor(ACharacter* Character) override;
	};

	// Indicates to the CMC that we will be using our custom FSavedMove_Nov
	class FNetworkPredictionData_Client_Nov : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Nov(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

	bool bWantsToWalk;
};
