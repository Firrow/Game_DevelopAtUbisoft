
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperTileMapActor.h"
#include "Test_GP_Map.generated.h"

class PaperTileMapComponent;
class UPaperTileSet;
class UPaperTileMap;
class UPaperTileLayer;


UENUM(BlueprintType)
enum class ETiles : uint8 {
	None = 0 UMETA(DisplayName = "None"),  // Valeur par défaut
	GROUND = 0 UMETA(DisplayName = "GROUND"),
	STARTBUILDING = 25 UMETA(DisplayName = "STARTBUILDING"),
	BUILDINGWALL = 26 UMETA(DisplayName = "BUILDINGWALL"),
	ENDBUILDING = 27 UMETA(DisplayName = "ENDBUILDING"),
	TEMP = 48 UMETA(DisplayName = "TEMP")
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
	int BuildingWidth = 0;

	ATest_GP_Map();
	void GenerateWorld();

	bool LeftNeighborIsNull(UPaperTileLayer& layer, int x, int y);
	bool CurrentTileIsOnGround(UPaperTileLayer& layer, int x, int y);
	bool BuildBuildingOrNot(int const probability);
	void CreateBuilding(int const x, int const y, UPaperTileLayer& layer);
	void PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer);

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "TileMap")
	UPaperTileSet* TileSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Settings")
	UPaperTileMapComponent* MyTileMapComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_START_BUILDING = 65;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_EXTEND_BUILD_WIDTH = PROBA_START_BUILDING;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int PROBA_EXTEND_BUILD_HEIGHT = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MIN_WIDTH_BUILDING = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_WIDTH_BUILDING = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MIN_HEIGHT_BUILDING = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_HEIGHT_BUILDING = 14;
};
