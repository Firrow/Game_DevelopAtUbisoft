
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

	ATest_GP_Map();
	void GenerateWorld();

	bool IsTileUserDataEqual(UPaperTileLayer& layer, int x, int y, FString tileType);
	bool IsTileNull(UPaperTileLayer& layer, int x, int y);
	int CountTiles(UPaperTileLayer& layer, int x, int y, TFunction<bool(UPaperTileLayer&, int, int)> condition, TFunction<void(int&, int&)> iteration);
	bool BuildOrNot(int const probability);
	void PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer);
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
	int MIN_WIDTH_BUILDING = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_WIDTH_BUILDING = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MIN_HEIGHT_BUILDING = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Map Settings")
	int MAX_HEIGHT_BUILDING = 14;
};
