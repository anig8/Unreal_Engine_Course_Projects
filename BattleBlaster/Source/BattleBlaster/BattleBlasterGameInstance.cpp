// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelInedx = Index;

		FString LevelNameString = FString::Printf(TEXT("Level_%d"), CurrentLevelInedx);
		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}
}
void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelInedx < LastLevelIndex)
	{
		ChangeLevel(CurrentLevelInedx + 1);
	}
	else
	{
		RestartGame();
	}
}
void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelInedx);
}
void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}