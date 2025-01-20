
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

    // ETAPE 5 : LAYER BUILDINGS - Placement des murs dans la grille
    for (int32 y = GridHeight; y >= 0; y--)
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

    // ETAPE 6 : LAYER INTERACTIBLES - Placement des éléments interactibles dans la map
    for (int32 y = GridHeight - MIN_HEIGHT_BUILDING; y >= 0; y--)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            // Vérification si on est en début de plateforme
            if (IsTileUserDataEqual(*BuildingLayer, x, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - 1, y, TEXT("GROUND"))) //si bord gauche plateforme et qu'une échelle ne vient pas d'être placée
            {
                // PLACEMENT DES ÉCHELLES
                if (!IsTileUserDataEqual(*BuildingLayer, x - 2, y, TEXT("GROUND")))
                {
                    CalculateLadderSpawnProbability(x, y, *BuildingLayer);
                }

            }
        }
    }

    // ETAPE 7 : AJOUT DU GAMEMANAGER
    GetWorld()->SpawnActor<AActor>(GameManager, FVector(0, 0, 0), FRotator::ZeroRotator);

    // ETAPE 8 : MAJ des collisions des tuiles
    MyTileMapComponent->RebuildCollision();
}



// TILE MANAGEMENT
/// <summary>
/// Return true or false depending on whether the given tile (in x ; y) has the same type than the given type
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="tileType"></param>
/// <returns></returns>
bool ATest_GP_Map::IsTileUserDataEqual(UPaperTileLayer& layer, int x, int y, FString tileType)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y);
    return TileInfoCell.TileSet && TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString() == tileType;
}

/// <summary>
/// Detect if given tile has asset associated
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool ATest_GP_Map::IsTileNull(UPaperTileLayer& layer, int x, int y)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y);
    return !TileInfoCell.TileSet;
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

/// <summary>
/// Eh you là-bas ! Near the betonneuse ! Put the ciment on the poteau please !
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="tile"></param>
/// <param name="layer"></param>
void ATest_GP_Map::PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer)
{
    FPaperTileInfo TileInfo;
    TileInfo.TileSet = TileSet;
    TileInfo.PackedTileIndex = tile;

    MyTileMapComponent->SetTile(x, y, layer.GetLayerIndex(), TileInfo);
}

/// <summary>
/// Takes a world position and convert it in tile coordinate if it's possible (useful for BP Tiles)
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
FVector ATest_GP_Map::ConvertGridPositionToWorldPosition(const int x, const int y)
{
    return FVector(x * TileSize, 0.0f, y * -TileSize);
}

/// <summary>
/// Spawn Blueprint tiles type (interactibles tiles)
/// </summary>
/// <param name="BPTile"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void ATest_GP_Map::SpawnBPTile(TSubclassOf<AInteractible>& BPTile, const int x, const int y)
{
    GetWorld()->SpawnActor<AActor>(BPTile, ConvertGridPositionToWorldPosition(x, y), FRotator::ZeroRotator);
}



// DECISION MANAGEMENT
/// <summary>
/// Return true or false depending on whether the given probability is upper than the random number between (0 ; 99)
/// If return is true, script can construct something, else it cannot
/// </summary>
/// <param name="probability"></param>
/// <returns></returns>
bool ATest_GP_Map::BuildOrNot(int const probability)
{
    return Stream.RandRange(0, 99) < probability;
}

/// <summary>
/// Check if a plateform is accessible from others or not. More returned int is big (between 0 and 6), more the plateform is isolated
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="currentPlateformLength"></param>
/// <returns></returns>
int ATest_GP_Map::PlateformIsAccessibleOrNot(UPaperTileLayer& layer, int x, int y, int const currentPlateformLength)
{
    int currentPlateformAvailability = 0;

    for (int i = 1; i <= 3; i++)
    {
        if (!IsTileUserDataEqual(layer, x - i, y + i, TEXT("GROUND")) && !IsTileUserDataEqual(layer, x - i, y, TEXT("GROUND")) && !IsTileUserDataEqual(layer, x - i, y - i, TEXT("GROUND")))
        {
            currentPlateformAvailability++;
        }
        if (!IsTileUserDataEqual(layer, x + currentPlateformLength + i, y + i, TEXT("GROUND")) && !IsTileUserDataEqual(layer, x + currentPlateformLength + i, y, TEXT("GROUND")) && !IsTileUserDataEqual(layer, x + currentPlateformLength + i, y - i, TEXT("GROUND")))
        {
            currentPlateformAvailability++;
        }
    }

    return currentPlateformAvailability;
}

/// <summary>
/// Check plateform accessibility and calculate a probability for this plateform to have a ladder
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="layer"></param>
void ATest_GP_Map::CalculateLadderSpawnProbability(int const x, int const y, UPaperTileLayer& layer)
{
    // 1) Calcul à quelle point la plateforme est accessible ou non depuis d'autres plateformes
    int currentPlateformAccessibility = PlateformIsAccessibleOrNot(layer, x, y, CountTiles(
        layer, x, y,
        [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y, TEXT("GROUND")) && IsTileUserDataEqual(layer, x + 1, y, TEXT("GROUND")); },
        [](int& x, int& y) { x++; }));

    // 2) Calcul la probabilité de spawn de l'échelle en fonction de l'accessibilité
    int proba_calculate = PROBA_LADDER;
    if (currentPlateformAccessibility == 6)
    {
        proba_calculate *= currentPlateformAccessibility;
    }

    // 3) Si l'échelle n'est pas accessible facilement, on regarde si l'on place une échelle ou non
    if (currentPlateformAccessibility >= VALUE_PLATEFORM_IS_NOT_AVAILABLE)
    {
        ChooseLadderSpawnPoint(layer, x, y, proba_calculate);
    }
}

/// <summary>
/// Decide, depending on draw, if it put a ladder on plateform or not
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="probability"></param>
void ATest_GP_Map::ChooseLadderSpawnPoint(UPaperTileLayer& layer, int x, int y, int const probability)
{
    bool isLadder = false;
    int i = 0;

    // Parcours la plateforme tant qu'il n'y a pas d'échelle
    while (!isLadder && IsTileUserDataEqual(layer, x + i, y, TEXT("GROUND")))
    {
        // S'il y a un mur en dessous du sol
        // et si les deux tuiles du dessous ne sont pas des sols
        // et si la probabilité de placer une échelle est bonne
        if (IsTileUserDataEqual(layer, x + i, y + 1, TEXT("BUILDINGWALL"))
            && (!IsTileUserDataEqual(layer, x + i, y - 1, TEXT("GROUND")) && !IsTileUserDataEqual(layer, x + i, y - 2, TEXT("GROUND")))
            && BuildOrNot(probability))
        {
            // Placer le haut de l'échelle et changer la tuile du dessous pour que l'échelle soit accessible par le joueur
            CreateLadder(layer, x + i, y);

            isLadder = true;
        }
        else
        {
            i++;
        }
    }
}



// CREATE ELEMENT IN MAP

/// <summary>
/// Put Ladder on map
/// </summary>
void ATest_GP_Map::CreateLadder(UPaperTileLayer& layer, int x, int y) //call x + i
{
    // First ladder's tile
    SpawnBPTile(Ladder, x, y);
    PutTileOnGrid(x, y, (int32)ETiles::UNDERLADDER, layer);

    int h = 1;

    while (!IsTileUserDataEqual(layer, x, y + h, TEXT("GROUND")))
    {
        SpawnBPTile(Ladder, x, y + h);
        h++;
    }
}

/// <summary>
/// Create an extention for the plateform (to his left)
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="layer"></param>
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

/// <summary>
/// Put the first tile on ground to create a building. This function allows ContinueBuilding() to construct the rest of the building
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="availableFloorSpace"></param>
/// <param name="layer"></param>
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

/// <summary>
/// Put tile on grid according to context. This function is structurated according to all available tiles to create building
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="layer"></param>
void ATest_GP_Map::ContinueBuilding(int const x, int const y, UPaperTileLayer& layer)
{
    // Construction en Hauteur (on est plus sur le sol)
    int heightBuilding = CountTiles(
        layer, x, y,
        [this](UPaperTileLayer& layer, int x, int y) -> bool { return !IsTileUserDataEqual(layer, x, y, TEXT("GROUND")); },
        [](int& x, int& y) { y++; });

    //STARTBUILDING
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
    //ENDBUILDING
    else if (IsTileUserDataEqual(layer, x, y + 1, TEXT("ENDBUILDING")) && !IsTileUserDataEqual(layer, x - 1, y, TEXT("GROUND")))
    {
        PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, layer);
    }
    //GROUND
    else if (
        //On pose un sol si
        // La tuile est à la dernière ligne de la grille et que la tuile du dessous n'est pas vide ou n'est pas un sol
        // OU La tuile en dessous n'est pas vide ET qu'elle n'est pas un sol
        // OU (corniche avant) que les 2 tuiles en dessous sont nulles
        //      ET que la tuile avant est un sol
        //      ET que la probabilité de poser un sol soit bonne
        ((y == 0 && !IsTileNull(layer, x, y + 1) && !IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")))
        || !IsTileNull(layer, x, y + 1) && (!IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND"))))
        || (IsTileNull(layer, x, y + 1) && IsTileNull(layer, x, y + 2)
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
