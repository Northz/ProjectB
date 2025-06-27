// Fill out your copyright notice in the Description page of Project Settings.


#include "NovCharacter.h"

// Sets default values
ANovCharacter::ANovCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANovCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANovCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANovCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

