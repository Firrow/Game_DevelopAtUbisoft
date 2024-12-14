
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "Map/Test_GP_Map.h"


ATest_GP_Map::ATest_GP_Map()
{
	GridWidth = 100;
	GridHeight = 100;
	TileSize = 16.f;

	MyTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
	RootComponent = MyTileMapComponent;
}

void ATest_GP_Map::BeginPlay()
{
	Super::BeginPlay();
	
	if (MyTileMapComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("TileMapComponent initialized"));
	}

	GenerateWorld();
}

void ATest_GP_Map::GenerateWorld()
{
    // Create a new TileMap
    UPaperTileMap* TileMap = NewObject<UPaperTileMap>(MyTileMapComponent);
    TileMap->Rename(TEXT("CityTileMap"));
    TileMap->MapWidth = GridWidth;
    TileMap->MapHeight = GridHeight;
    TileMap->TileWidth = TileSize;
    TileMap->TileHeight = TileSize;
    TileMap->PixelsPerUnrealUnit = 1.0f;
    TileMap->SeparationPerTileX = 0.0f;
    TileMap->SeparationPerTileY = 0.0f;
    TileMap->SeparationPerLayer = 0.0f;
    MyTileMapComponent->SetTileMap(TileMap);

    if (MyTileMapComponent->OwnsTileMap())
    {
        UE_LOG(LogTemp, Warning, TEXT("TileMap is now owned by MyTileMapComponent"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to make MyTileMapComponent own the TileMap!"));
        return;
    }

    UPaperTileLayer* NewLayer = TileMap->AddNewLayer();

    for (int32 x = 0; x < GridWidth; x++)
    {
        for (int32 y = 0; y < GridHeight; y++)
        {
            FPaperTileInfo TileInfo;
            TileInfo.TileSet = TileSet;
            TileInfo.PackedTileIndex = 0;

            MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);

            UE_LOG(LogTemp, Warning, TEXT("NOM TILESET %s"), *TileInfo.TileSet.GetName());
            UE_LOG(LogTemp, Warning, TEXT("tuile definie a (%d, %d) avec l'index %i"), x, y, TileInfo.PackedTileIndex);
            UE_LOG(LogTemp, Warning, TEXT("UserDataName : %s"), *TileSet->GetTileUserData(TileInfo.PackedTileIndex).ToString());
        }
    }

    MyTileMapComponent->MarkRenderStateDirty();
    MyTileMapComponent->RebuildCollision();
}
