// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectInGame/Ressources/Watch_ressource.h"

void AWatch_ressource::RessourceEffect()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("WATCH !!!!!!!!!!!!!!!!"));
}