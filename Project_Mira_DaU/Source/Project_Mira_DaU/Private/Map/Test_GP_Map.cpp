
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "Math/UnrealMathUtility.h"
#include "Templates/Function.h"
#include "Map/Test_GP_Map.h"


// ETAPE 1 : Initialiser la TileMapComponent et les valeurs qui seront utilisées
ATest_GP_Map::ATest_GP_Map()
{
	GridWidth = 100;
	GridHeight = 100; 
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

    if (SEED.randomSeed)
    {
        SEED.value = FMath::Rand();
        Stream.Initialize(SEED.value);
    }
    else
    {
        Stream.Initialize(SEED.value);
    }
    GEngine->AddOnScreenDebugMessage(-1, 50000.f, FColor::Yellow, FString::Printf(TEXT("SEED : %d"), SEED.value));

    // ETAPE 4 : Création d'un nouveau calque pour poser les tuiles
    UPaperTileLayer* NewLayer = TileMap->AddNewLayer();

    // ETAPE 5 : Placement des tuiles dans la grille pour le test
    for (int32 y = GridHeight - 1; y >= 0; y--)
    {
        int BuildingWidth = 0;
        int availableFloorSpace = 0;

        for (int32 x = 0; x < GridWidth; x++)
        {
            if (y == GridHeight - 1)
            {
                PutTileOnGrid(x, y, (int32)ETiles::GROUND, *NewLayer);
            }
            else 
            {
                if (y >= MIN_HEIGHT_BUILDING && IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("GROUND")))
                {
                    CreateBuilding(x, y, BuildingWidth, availableFloorSpace, *NewLayer);
                }
                else
                {   
                    ContinueBuilding(x, y, *NewLayer);
                }
            }
        }
    }

    // ETAPE 6 : MAJ des collisions des tuiles
    MyTileMapComponent->RebuildCollision();
}

bool ATest_GP_Map::IsTileUserDataEqual(UPaperTileLayer& layer,  int x, int y, FString tileType)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y);
    return TileInfoCell.TileSet && TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString() == tileType;
}
 
bool ATest_GP_Map::IsTileNull(UPaperTileLayer& layer, int x, int y)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y);
    return !TileInfoCell.TileSet;
}

bool ATest_GP_Map::BuildOrNot(int const probability)
{
    return Stream.RandRange(0, 99) < probability;
}

/// <summary>
/// Count all tiles that check condition along the iteration vector
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="condition"></param>
/// <param name="iteration"></param>
/// <returns></returns>
int ATest_GP_Map::CountTiles(UPaperTileLayer& layer, int x, int y, TFunction<bool(UPaperTileLayer&, int, int)> condition, TFunction<void(int&, int&)> iteration)
{
    if (x >= 0 && x < GridWidth && y >= 0 && y < GridHeight && condition(layer, x, y))
    {
        iteration(x, y);
        return CountTiles(layer, x, y, condition, iteration) + 1;
    }
    else
    {
        return 0;
    }
}

// Eh you là-bas ! Near the betonneuse ! Put the ciment on the poteau please !
void ATest_GP_Map::PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer)
{
    FPaperTileInfo TileInfo;
    TileInfo.TileSet = TileSet;
    TileInfo.PackedTileIndex = tile;

    MyTileMapComponent->SetTile(x, y, layer.GetLayerIndex(), TileInfo);
}

void ATest_GP_Map::CreateBuilding(int const x, int const y, int& width, int& availableFloorSpace, UPaperTileLayer& layer)
{
    if (width == 0)
    {
        availableFloorSpace = CountTiles(
            layer, x, y,
            [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")); },
            [](int& x, int& y) { x++; });

        // Commencer nouveau batiment : poser une tuile début de bâtiment
        if (availableFloorSpace >= MIN_WIDTH_BUILDING && BuildOrNot(PROBA_START_BUILDING))
        {
            PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, layer);

            width = 1;
            availableFloorSpace--;
        }
    }
    else
    {
        width++;
        availableFloorSpace--;

        // Si je suis encore dans la grille et que 
        // soit je suis en dessous de la taille minimal requise 
        // soit que je suis inférieur à la taille max et que la probabilité dit de continuer le bâtiment
        if (availableFloorSpace > 0
            && (width < MIN_WIDTH_BUILDING ||
               (width < MAX_WIDTH_BUILDING && BuildOrNot(PROBA_EXTEND_BUILD_WIDTH)))
            )
        {
            PutTileOnGrid(x, y, (int32)ETiles::BUILDINGWALL, layer);
        }
        else
        {
            PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, layer);

            width = 0;
        }
    }
}

void ATest_GP_Map::ContinueBuilding(int const x, int const y, UPaperTileLayer& layer)
{
    // Construction en Hauteur (on est pas sur le sol)
    //START (ou GROUND)
    if (IsTileUserDataEqual(layer, x, y + 1, TEXT("STARTBUILDING")))
    {
        // On est au début d'un autre batiment
        int numberOfTiles = CountTiles(
            layer, x, y,
            [this](UPaperTileLayer& layer, int x, int y) -> bool { return !IsTileUserDataEqual(layer, x, y, TEXT("GROUND")); },
            [](int& x, int& y) { y++; });

        if (numberOfTiles <= MIN_HEIGHT_BUILDING)
        {
            PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, layer);
        }
        else if (numberOfTiles == MAX_HEIGHT_BUILDING)
        {
            PutTileOnGrid(x, y, (int32)ETiles::GROUND, layer);

            // BACK LEDGES
            int numberOfTile = CountTiles(
                layer, x - 1, y,
                [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileNull(layer, x, y); },
                [](int& x, int& y) { x--; });

            if (numberOfTile > 0)
            {
                int xBackLedge = x - 1;

                while (IsTileNull(layer, xBackLedge, y) && BuildOrNot(100 - numberOfTile * 8))
                {
                    PutTileOnGrid(xBackLedge, y, (int32)ETiles::GROUND, layer);
                    xBackLedge--;
                }
            }
        }
        else
        {
            // probabilité de continuer le bâtiment en hauteur
            if (BuildOrNot(PROBA_EXTEND_BUILD_HEIGHT))
            {
                PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, layer);
            }
            else
            {
                PutTileOnGrid(x, y, (int32)ETiles::GROUND, layer);

                // BACK LEDGES
                int numberOfTile = CountTiles(
                    layer, x - 1, y,
                    [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileNull(layer, x, y); },
                    [](int& x, int& y) { x--; });

                if (numberOfTile > 0)
                {
                    int xBackLedge = x - 1;

                    while (IsTileNull(layer, xBackLedge, y) && BuildOrNot(100 - numberOfTile * 8))
                    {
                        PutTileOnGrid(xBackLedge, y, (int32)ETiles::GROUND, layer);
                        xBackLedge--;
                    }
                }
            }
        }
    }
    //GROUND
    else if (IsTileUserDataEqual(layer, x - 1, y, TEXT("GROUND")))
    {
        FPaperTileInfo TileInfoCell = layer.GetCell(x, y + 1);

        if (!TileInfoCell.TileSet)
        {
            // FRONT LEDGES
            int numberOfTile = CountTiles(
                layer, x - 1, y,
                [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y, TEXT("GROUND")); },
                [](int& x, int& y) { x--; });

            if (BuildOrNot(numberOfTile * 5))
            {
                PutTileOnGrid(x, y, (int32)ETiles::GROUND, layer);
            }
        }
        else
        {
            PutTileOnGrid(x, y, (int32)ETiles::GROUND, layer);
        }
    }
    //END
    else if (IsTileUserDataEqual(layer, x, y + 1, TEXT("ENDBUILDING")))
    {
        PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, layer);
    }
    //WALL
    else if (IsTileUserDataEqual(layer, x, y + 1, TEXT("BUILDINGWALL")))
    {
        PutTileOnGrid(x, y, (int32)ETiles::BUILDINGWALL, layer);
    }
}