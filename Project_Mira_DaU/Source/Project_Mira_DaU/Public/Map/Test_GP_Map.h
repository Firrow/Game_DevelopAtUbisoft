
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Test_GP_Map.generated.h"

class PaperTileMapComponent;
class UPaperTileSet;
class UPaperTileMap;

UCLASS()
class PROJECT_MIRA_DAU_API ATest_GP_Map : public AActor
{
	GENERATED_BODY()
		
private:
	UPaperTileMapComponent* TileMapComponent;
	int GridWidth;
	int GridHeight;
	float TileSize;

	ATest_GP_Map();
	void GenerateWorld();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "TileMap")
	UPaperTileSet* TileSet;

	/*UPROPERTY(EditAnywhere, Category = "TileMap")
	UPaperTileMap* MyTileMap;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
	UPaperTileMapComponent* MyTileMapComponent;

};
