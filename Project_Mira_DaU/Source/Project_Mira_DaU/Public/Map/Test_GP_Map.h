
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "ObjectInGame/Interactible.h"
#include "ObjectInGame/Ressource.h"
#include "Game/GameManager.h"
#include "Test_GP_Map.generated.h"

class PaperTileMapComponent;
class UPaperTileSet;
class UPaperTileMap;
class UPaperTileLayer;
class AGameManager;


UENUM(BlueprintType)
enum class ETiles : uint8 {
	None = 0 UMETA(DisplayName = "None"),  // Valeur par défaut
	GROUND = 0 UMETA(DisplayName = "GROUND"),
	STARTBUILDING = 25 UMETA(DisplayName = "STARTBUILDING"),
	BUILDINGWALL = 26 UMETA(DisplayName = "BUILDINGWALL"),
	ENDBUILDING = 27 UMETA(DisplayName = "ENDBUILDING"),
	UNDERLADDER = 22 UMETA(DisplayName = "UNDERLADDER"),
	TEMP = 48 UMETA(DisplayName = "TEMP")
};


USTRUCT(BlueprintType) 
struct FSeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings", meta = (EditCondition = "!randomSeed"))
	int32 value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	bool randomSeed;
};


UCLASS()
class PROJECT_MIRA_DAU_API ATest_GP_Map : public AActor
{
	GENERATED_BODY()
		
private:
	UPaperTileMapComponent* TileMapComponent;
	int GridWidth;
	int GridHeight;
	float TileSize;
	FRandomStream Stream; //Ma map étant une classe de generation, je prefere le mettre en attribut (Stream <=> RNG)
	AGameManager* GameManager;
	TSet<FVector2D> BPPositionInGrid;
	int TotalRessourcesQuantity;

	ATest_GP_Map();
	void GetGameManager();
	void CalculateTotalRessourcesQuantity();
    void GenerateWorld();

	bool IsTileUserDataEqual(UPaperTileLayer& layer, int x, int y, FString tileType);
	bool IsTileNull(UPaperTileLayer& layer, int x, int y);
	int CountTiles(UPaperTileLayer& layer, int x, int y, TFunction<bool(UPaperTileLayer&, int, int)> condition, TFunction<void(int&, int&)> iteration);
	void PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer);
	FVector ConvertGridPositionToWorldPosition(const int x, const int y);
	void SpawnBPTile(TSubclassOf<AInteractible>& BPTile, const int x, const int y);
	AInteractible* FindInteractibleAtGridPosition(int x, int y);

	bool BuildOrNot(int const probability);
	int PlateformIsAccessibleOrNot(UPaperTileLayer& layer, int x, int y, int const currentPlateformLength);
	void CalculateLadderSpawnProbability(int const x, int const y, UPaperTileLayer& layer);
	void ChooseLadderSpawnPoint(UPaperTileLayer& layer, int x, int y, int const probability);

	void CreateLadder(UPaperTileLayer& layer, int x, int y);
	void CreateContainer(UPaperTileLayer& layer, FIntPoint& coordinates);
	void CreateBackLedge(int const x, int const y, UPaperTileLayer& layer);
	void CreateBuilding(int const x, int const y, int& width, int& availableFloorSpace, UPaperTileLayer& layer);
	void ContinueBuilding(int const x, int const y, UPaperTileLayer& layer);

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "TileMap")
	UPaperTileSet* TileSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
	UPaperTileMapComponent* MyTileMapComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	FSeed SEED = { 0, false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_START_BUILDING = 85;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_EXTEND_BUILD_WIDTH = PROBA_START_BUILDING;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_EXTEND_BUILD_HEIGHT = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_BACK_LEDGE = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_FRONT_LEDGE = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_LADDER = 8;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MIN_WIDTH_BUILDING = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_WIDTH_BUILDING = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MIN_HEIGHT_BUILDING = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_HEIGHT_BUILDING = 14;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int VALUE_PLATEFORM_IS_NOT_AVAILABLE = 4;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element in Map")
	TSubclassOf<AInteractible> Ladder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element in Map")
	TSubclassOf<AInteractible> Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Element in Map")
	TMap<int, TSubclassOf<ARessource>> RessourcesType;
};
