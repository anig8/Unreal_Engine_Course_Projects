// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*>Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("Game mode failed to find the tank actor!"));
		}
	}

	int32 LoopIndex = 0;
	while (LoopIndex < TowerCount) 
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank) {
				Tower->Tank = Tank;
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variable!"), *Tower->GetActorNameOrLabel());
			}
		}

		LoopIndex++;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		ScreenMeesageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);

		if (ScreenMeesageWidget)
		{
			ScreenMeesageWidget->AddToPlayerScreen();
			ScreenMeesageWidget->SetMessageText("Get Ready!");
		}
	}


	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds -= 1;

	if (CountdownSeconds > 0)
	{
		ScreenMeesageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		ScreenMeesageWidget->SetMessageText("Go!");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		ScreenMeesageWidget->SetVisibility(ESlateVisibility::Hidden);
	}

}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;
	
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else 
	{
		// A tower died
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower -> HandleDestruction();
			
			TowerCount--;
			if (TowerCount == 0)
			{
				IsVictory = true;
				IsGameOver = true;
			}
		}
	}
	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		ScreenMeesageWidget->SetMessageText(GameOverString);
		ScreenMeesageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance =  GetGameInstance();
		if (GameInstance)
		{
			UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);

			if (BattleBlasterGameInstance)
			{
				if (IsVictory)
				{
					BattleBlasterGameInstance->LoadNextLevel();
				}
				else
				{
					BattleBlasterGameInstance->RestartCurrentLevel();
				}
			}			
		}
}
