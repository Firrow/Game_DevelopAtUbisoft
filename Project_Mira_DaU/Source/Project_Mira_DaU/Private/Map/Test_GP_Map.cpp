
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

    UE_LOG(LogTemp, Warning, TEXT("SEED : % d"), SEED.value);
	
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
    UPaperTileLayer* BuildingLayer = TileMap->AddNewLayer();

    // ETAPE 5 : Placement des murs dans la grille
    for (int32 y = GridHeight; y >= 0; y--) // - 1
    {
        int BuildingWidth = 0;
        int availableFloorSpace = 0;

        for (int32 x = 0; x < GridWidth; x++)
        {
            if (y == GridHeight - 1)
            {
                PutTileOnGrid(x, y, (int32)ETiles::GROUND, *BuildingLayer);
            }
            else 
            {
                if (y >= MIN_HEIGHT_BUILDING && IsTileUserDataEqual(*BuildingLayer, x, y + 1, TEXT("GROUND")))
                {
                    CreateBuilding(x, y, BuildingWidth, availableFloorSpace, *BuildingLayer);
                }
                else
                {   
                    ContinueBuilding(x, y, *BuildingLayer);
                }
            }
        }
    }

    // ETAPE 6 : Placement des éléments interactibles dans la map
    for (int32 y = GridHeight - 2; y >= 0; y--) // - 1
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            if (IsTileUserDataEqual(*BuildingLayer, x, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - 1, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - 2, y, TEXT("GROUND"))) //si bord gauche plateforme et qu'une échelle ne vient pas d'être placée
            {
                int isNotAvailable = 0;
                //calcul disponibilité à gauche
                for (int i = 1; i <= 3; i++)
                {
                    // check if a ground is available in player jump range (down only)
                    if (!IsTileUserDataEqual(*BuildingLayer, x - i, y + i, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - i, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - i, y - i, TEXT("GROUND")))
                    {
                        isNotAvailable++;
                    }
                }

                int plateformLength = CountTiles(
                    *BuildingLayer, x, y,
                    [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y, TEXT("GROUND")) && IsTileUserDataEqual(layer, x + 1, y, TEXT("GROUND")); },
                    [](int& x, int& y) { x++; });


                for (int j = 1; j <= 3; j++)
                {
                    // check if a ground is available in player jump range (down only)
                    if (!IsTileUserDataEqual(*BuildingLayer, x + plateformLength + j, y + j, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x + plateformLength + j, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x + plateformLength + j, y - j, TEXT("GROUND")))
                    {
                        isNotAvailable++;
                    }
                }


                int proba_calculate;
                if (isNotAvailable == 6)
                {
                    proba_calculate = PROBA_LADDER * isNotAvailable;
                }
                else
                {
                    proba_calculate = PROBA_LADDER;
                }
                

                if (isNotAvailable >= 4)
                {
                    bool isLadder = false;
                    int k = 0;

                    while (!isLadder && IsTileUserDataEqual(*BuildingLayer, x + k, y, TEXT("GROUND")))
                    {
                        if (IsTileUserDataEqual(*BuildingLayer, x + k, y + 1, TEXT("BUILDINGWALL"))
                            && (IsTileUserDataEqual(*BuildingLayer, x + k, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x + k, y - 1, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x + k, y - 2, TEXT("GROUND")))
                            && BuildOrNot(proba_calculate))
                        {
                            SpawnBPTile(Ladder, x + k, y);
                            PutTileOnGrid(x + k, y, (int32)ETiles::UNDERLADDER, *BuildingLayer);

                            isLadder = true;

                            int l = 1;

                            while (!IsTileUserDataEqual(*BuildingLayer, x + k, y + l, TEXT("GROUND")))
                            {
                                SpawnBPTile(Ladder, x + k, y + l);
                                l++;
                            }
                        }
                        else
                        {
                            k++;
                        }
                    }

                    k = 0;
                    isLadder = false;
                }

                isNotAvailable = 0;
            }
        }
    }

    // ETAPE 7 : MAJ des collisions des tuiles
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

void ATest_GP_Map::CreateBackLedge(int const x, int const y, UPaperTileLayer& layer)
{

    // BACK LEDGES : On prend en compte le nombre de tuile vide précédente pour avoir plus de chance d'avoir une corniche arrière si le nombre de tuile vide est faible
    int numberOfTile = CountTiles(
        layer, x - 1, y,
        [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileNull(layer, x, y); },
        [](int& x, int& y) { x--; });

    if (numberOfTile > 0)
    {
        int xBackLedge = x - 1;

        while (IsTileNull(layer, xBackLedge, y) && BuildOrNot(100 - numberOfTile * PROBA_BACK_LEDGE))
        {
            PutTileOnGrid(xBackLedge, y, (int32)ETiles::GROUND, layer);
            xBackLedge--;
        }
    }
}

FVector ATest_GP_Map::ConvertGridPositionToWorldPosition(const int x, const int y)
{
    return FVector(x * TileSize, 0.0f, y * -TileSize);
}

void ATest_GP_Map::SpawnBPTile(TSubclassOf<AInteractible>& BPTile, const int x, const int y)
{
    GetWorld()->SpawnActor<AActor>(BPTile, ConvertGridPositionToWorldPosition(x, y), FRotator::ZeroRotator);
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
    int heightBuilding = CountTiles(
        layer, x, y,
        [this](UPaperTileLayer& layer, int x, int y) -> bool { return !IsTileUserDataEqual(layer, x, y, TEXT("GROUND")); },
        [](int& x, int& y) { y++; });

    //START
    if (y > 0 && IsTileUserDataEqual(layer, x, y + 1, TEXT("STARTBUILDING"))
        && (heightBuilding <= MIN_HEIGHT_BUILDING
            || (heightBuilding < MAX_HEIGHT_BUILDING && BuildOrNot(PROBA_EXTEND_BUILD_HEIGHT))))
    {
        // On est au début d'un autre batiment
        PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, layer);
    }
    //WALL
    else if (IsTileUserDataEqual(layer, x, y + 1, TEXT("BUILDINGWALL")) && !IsTileUserDataEqual(layer, x - 1, y, TEXT("GROUND")))
    {
        PutTileOnGrid(x, y, (int32)ETiles::BUILDINGWALL, layer);
    }
    //END
    else if (IsTileUserDataEqual(layer, x, y + 1, TEXT("ENDBUILDING")) && !IsTileUserDataEqual(layer, x - 1, y, TEXT("GROUND")))
    {
        PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, layer);
    }
    //GROUND
    else if (
        //On pose un sol si
        // La tuile en dessous n'est pas vide ET qu'elle n'est pas un sol
        // OU (corniche avant) que la tuile en dessous est nulle
        //      ET que la tuile avant est un sol
        //      ET que la probabilité de poser un sol soit bonne
        ((y == 0 && !IsTileNull(layer, x, y + 1) && !IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")))
            || !IsTileNull(layer, x, y + 1) && (!IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND"))))
        || (IsTileNull(layer, x, y + 1)
            && IsTileNull(layer, x, y + 2)
            && IsTileUserDataEqual(layer, x - 1, y, TEXT("GROUND"))
            && BuildOrNot(CountTiles(
                layer, x - 1, y,
                [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y, TEXT("GROUND")); },
                [](int& x, int& y) { x--; }) * PROBA_FRONT_LEDGE)))
        // FRONT LEDGE : On prend en compte le nombre de tuile sol précédente pour avoir une taille de corniche cohérente avec la taille du batiment
    {

        // Placer une mur en dessous de la ground
        PutTileOnGrid(x, y, (int32)ETiles::GROUND, layer);

        if (IsTileUserDataEqual(layer, x, y + 1, TEXT("STARTBUILDING")))
        {
            CreateBackLedge(x, y, layer);
        }
    }
}