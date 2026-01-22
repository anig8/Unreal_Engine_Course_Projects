// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
{
	if (Tank && Tank->IsAlive && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool Results = false;
	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		Results = (DistanceToTank <= FireRange);
	}
	
	return Results;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
