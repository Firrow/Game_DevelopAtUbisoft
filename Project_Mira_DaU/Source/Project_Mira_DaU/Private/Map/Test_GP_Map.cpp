
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "Math/UnrealMathUtility.h"
#include "Map/Test_GP_Map.h"


// ETAPE 1 : Initialisée la TileMapComponent et les valeurs qui seront utilisées
ATest_GP_Map::ATest_GP_Map()
{
	GridWidth = 50;
	GridHeight = 50; 
	TileSize = 16.f;

	MyTileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("TileMapComponent"));
	RootComponent = MyTileMapComponent;
}

// ETAPE 2 : Lancer la génération du monde
void ATest_GP_Map::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateWorld();
}

void ATest_GP_Map::GenerateWorld()
{
    // ETAPE 3 : Création et assignation d'une TileMap avec pour propriétaire la TileMapComponent
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


    // ETAPE 4 : Création d'un nouveau calque pour poser les tuiles
    UPaperTileLayer* NewLayer = TileMap->AddNewLayer();

    // ETAPE 5 : Placement des tuiles dans la grille pour le test
    for (int32 y = GridHeight - 1; y >= 0; y--)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            //Put ground on 
            if (y == GridHeight - 1)
            {
                PutTileOnGrid(x, y, (int32)ETiles::GROUND, *NewLayer);
            }
            else 
            {
                if (CurrentTileIsOnGround(*NewLayer, x, y))
                {
                    CreateBuilding(x, y, *NewLayer);
                }
                else
                {
                    PutTileOnGrid(x, y, (int32)ETiles::TEMP, *NewLayer);
                }
            }
        }
    }

    // ETAPE 6 : MAJ des collisions des tuiles
    MyTileMapComponent->RebuildCollision();
}

bool ATest_GP_Map::LeftNeighborIsNull(UPaperTileLayer& layer, int x, int y)
{
    return layer.GetCell(x - 1, y).PackedTileIndex == INDEX_NONE || layer.GetCell(x, y).PackedTileIndex == NULL;
}

bool ATest_GP_Map::CurrentTileIsOnGround(UPaperTileLayer& layer,  int x, int y)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y + 1);

    return TileInfoCell.TileSet && TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString() == TEXT("Ground");
}

bool ATest_GP_Map::BuildBuildingOrNot(int const probability)
{
    return FMath::RandRange(0, 99) < probability;
}

void ATest_GP_Map::CreateBuilding(int const x, int const y, UPaperTileLayer& layer)
{
    if (BuildingWidth == 0)
    {
        // Commencer nouveau batiment
        if (GridWidth - x >= MIN_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_START_BUILDING))
        {
            PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, layer);

            BuildingWidth = 1;
        }
    }
    else
    {
        BuildingWidth++;

        // Si je suis encore dans la grille et que soit je suis en dessous de la taille minimal requise ou que je suis inférieur à la 
        // taille max et que la probabilité de continuer le batiment demande de continuer le batiment
        if (x < GridWidth
            && (BuildingWidth < MIN_WIDTH_BUILDING ||
                (BuildingWidth < MAX_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_EXTEND_BUILD_WIDTH)))
            )
        {
            PutTileOnGrid(x, y, (int32)ETiles::BUILDINGWALL, layer);
        }
        else
        {
            PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, layer);

            BuildingWidth = 0;
        }
    }
}

// Eh you là-bas ! Near the bétoneuse ! Put the ciment on the poteau please !
void ATest_GP_Map::PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer)
{
    FPaperTileInfo TileInfo;
    TileInfo.TileSet = TileSet;
    TileInfo.PackedTileIndex = tile;

    // Placement de la tuile
    MyTileMapComponent->SetTile(x, y, layer.GetLayerIndex(), TileInfo);

}