
#include "Map/Test_GP_Map.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "Math/UnrealMathUtility.h"
#include "Templates/Function.h"
#include "Kismet/GameplayStatics.h"

#include "ObjectInGame/Container.h"
#include "Game/TriggerEnding.h"
#include "Characters/PlayerCharacter.h"


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

    DeleteFirstPlayerInstance();

    GetGameManager();
    CalculateTotalRessourcesQuantity();
	
	GenerateWorld();
    GenerateBackground();
}

void ATest_GP_Map::GetGameManager()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("World is not available!"));
    }

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, FName("GameManager"), FoundActors);
    if (FoundActors.Num() > 0)
    {
        GameManager = Cast<AGameManager>(FoundActors[0]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Actor founded!"));
    }
}

void ATest_GP_Map::CalculateTotalRessourcesQuantity()
{
    TotalRessourcesQuantity = 0;

    for (int i = 0; i < GameManager->RessourcesQuantity.Num(); i++)
    {
        TotalRessourcesQuantity += GameManager->RessourcesQuantity[i];
    }
}

// MAP INITIALIZATION
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
    // PLACEMENT DES ÉCHELLES
    for (int32 y = GridHeight - MIN_HEIGHT_BUILDING; y >= 0; y--)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            if (IsTileUserDataEqual(*BuildingLayer, x, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - 1, y, TEXT("GROUND")) && !IsTileUserDataEqual(*BuildingLayer, x - 2, y, TEXT("GROUND")))
            {
                CalculateLadderSpawnProbability(x, y, *BuildingLayer);
            }
        }
    }

    //PLACEMENT DES PORTES
    for (int32 y = GridHeight - 1; y >= 0; y--)
    {
        for (int32 x = 0; x < GridWidth; x++)
        {
            CreateDoor(*BuildingLayer, x, y);
        }
    }

    //PLACEMENT DES COFFRES
    for (int i = 1; i <= TotalRessourcesQuantity; i++)
    {
        ChooseContainerSpawnPoint(*BuildingLayer);
    }

    // ETAPE 7 : PLACEMENT DES ÉLÉMENTS DE DÉBUT ET FIN DE PARTIE 
    CreateTriggerEnding(*BuildingLayer);

    // ETAPE 8 : MAJ des collisions des tuiles
    MyTileMapComponent->RebuildCollision();

    // ÉTAPE 9 : Ajouter le joueur à la map
    SpawnPlayerInScene(*BuildingLayer);
}

void ATest_GP_Map::GenerateBackground()
{   
    int xBackground = GridWidth / 2;
    int yBackground = GridHeight / 2;
    FActorSpawnParameters SpawnParams;
    GetWorld()->SpawnActor<AActor>(Background, ConvertGridPositionToWorldPosition(xBackground, yBackground, false), FRotator::ZeroRotator, SpawnParams);
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
FVector ATest_GP_Map::ConvertGridPositionToWorldPosition(const int x, const int y, bool isPlayer = false)
{
    return isPlayer ? FVector(x * TileSize, 0.f, y * -TileSize) : FVector(x * TileSize, -0.1f, y * -TileSize);
}

/// <summary>
/// Spawn Blueprint tiles type (interactibles tiles) and add coordinates in BPPositionInGrid
/// </summary>
/// <param name="BPTile"> BP to spawn </param>
/// <param name="BPSize"> Number of tiles in BP width </param>
/// <param name="x"> BP coordinate X </param>
/// <param name="y"> BP coordinate Y </param>
/// <param name="xOffset"> Value difference between BP coordinate X and value to put in BPPositionInGrid </param>
/// <param name="yOffset"> Value difference between BP coordinate Y and value to put in BPPositionInGrid </param>
template<typename T>
void ATest_GP_Map::SpawnBPTile(TSubclassOf<T>& BPTile, int BPSize, const int x, const int y, const int xOffset, const int yOffset)
{
    GetWorld()->SpawnActor<T>(BPTile, ConvertGridPositionToWorldPosition(x, y), FRotator::ZeroRotator);

    for (int32 i = 0; i < BPSize; i++)
    {
        BPPositionInGrid.Add(FIntPoint(x + xOffset + i, y + yOffset));
    }
}

/// <summary>
/// Get the BP instance on a given tile
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
AActor* ATest_GP_Map::FindInteractibleAtGridPosition(int x, int y)
{
    FVector WorldPosition = ConvertGridPositionToWorldPosition(x, y);

    // Définir un rayon pour rechercher autour de la position
    float SearchRadius = TileSize * 0.5f;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SearchRadius);

    // Configuration pour ne rechercher que les acteurs dynamiques (par exemple, les BP instanciés)
    FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_WorldDynamic);

    TArray<FOverlapResult> OverlapResults;

    // Effectuer une recherche d'overlap
    bool bHasHit = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        WorldPosition,
        FQuat::Identity,
        ObjectQueryParams,
        CollisionShape
    );

    if (bHasHit)
    {
        // Parcourir les résultats et vérifier le type
        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* Actor = Result.GetActor();
            return Actor;
        }
    }

    return nullptr;
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

/// <summary>
/// 
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="probability"></param>
void ATest_GP_Map::ChooseContainerSpawnPoint(UPaperTileLayer& layer)
{
    // 1) Tirer au hasard des coordonnées
    std::unique_ptr<FIntPoint> coordinates =
        std::make_unique<FIntPoint>(Stream.RandRange(0, GridWidth - 1), Stream.RandRange(0, GridHeight - 1));

    // 2) Ajuster les coordonnées du container
    bool isOnGround = false;
    bool isOnBP = true;
    int attempts = 0;

    while (!isOnGround || isOnBP)
    {
        // check if container is on ground and not between two grounds
        if (!IsTileUserDataEqual(layer, coordinates->X, coordinates->Y + 1, TEXT("GROUND"))
            || IsTileUserDataEqual(layer, coordinates->X, coordinates->Y - 1, TEXT("GROUND")))
        {
            coordinates->Y += 1;
            isOnGround = false;
        }
        else
        {
            isOnGround = true;
        }

        // check if container is on another BP
        if (BPPositionInGrid.Contains(FIntPoint(coordinates->X, coordinates->Y)))
        {
            IsTileUserDataEqual(layer, coordinates->X + 1, coordinates->Y + 1, TEXT("GROUND")) ? coordinates->X += 1 : coordinates->Y = (coordinates->Y + 1) % GridHeight;
            isOnBP = true;
        }
        else
        {
            isOnBP = false;
        }

        attempts++;

        if (attempts >= 200)
        {
            coordinates->X = Stream.RandRange(0, GridWidth - 1);
            coordinates->Y = Stream.RandRange(0, GridHeight - 1);
            isOnGround = false;
            isOnBP = true;
            attempts = 0;
        }
    }

    // 3) Placement du coffre
    CreateContainer(layer, *coordinates);
}



// CREATE ELEMENT IN MAP
/// <summary>
/// Put ladder on map
/// </summary>
void ATest_GP_Map::CreateLadder(UPaperTileLayer& layer, int x, int y)
{
    // First ladder's tile
    SpawnBPTile(Ladder, 1, x, y);
    PutTileOnGrid(x, y, (int32)ETiles::UNDERLADDER, layer);

    int h = 1;

    while (!IsTileUserDataEqual(layer, x, y + h, TEXT("GROUND")))
    {
        SpawnBPTile(Ladder, 1, x, y + h);
        h++;
    }
}

/// <summary>
/// Put door on map
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="layer"></param>
void ATest_GP_Map::CreateDoor(UPaperTileLayer& layer, int const x, int const y)
{
    bool doorIsPlaced = false;

    if (!doorIsPlaced && IsTileUserDataEqual(layer, x, y, TEXT("STARTBUILDING")) && IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")))
    {
        // calculate number of tiles between starbuilding + 1 and endbuilding - 2 (door takes 2 tiles)
        int availableSpace = CountTiles(
            layer, x + 1, y,
            [this](UPaperTileLayer& layer, int x, int y) -> bool { return IsTileUserDataEqual(layer, x, y, TEXT("BUILDINGWALL")); },
            [](int& x, int& y) { x++; }) - 2;

        TArray<int> xTileWithSequenceAvailable = {};

        // Get all X when x, x+1 and x+2 have no BP on it and put X value in xTileWithSequenceAvailable
        for (int i = 1; i < availableSpace; i++)
        {
            if (!FindInteractibleAtGridPosition(x + i, y) && !IsTileUserDataEqual(layer, x + i, y, TEXT("ENDBUILDING")) &&
                !FindInteractibleAtGridPosition(x + i + 1, y) && !IsTileUserDataEqual(layer, x + i + 1, y, TEXT("ENDBUILDING")) &&
                !FindInteractibleAtGridPosition(x + i + 2, y) && !IsTileUserDataEqual(layer, x + i + 2, y, TEXT("ENDBUILDING")))
            {
                xTileWithSequenceAvailable.Add(x + i);
            }
        }

        if (xTileWithSequenceAvailable.Num() > 0)
        {
            // Get random index to choose X value in xTileWithSequenceAvailable
            int indexInSequence = Stream.RandRange(0, xTileWithSequenceAvailable.Num() - 1);

            SpawnBPTile(Door, 3, xTileWithSequenceAvailable[indexInSequence], y - 1, 0, 1);

            doorIsPlaced = true;
        }

        xTileWithSequenceAvailable = {};
    }
}

/// <summary>
/// Put chest on map
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void ATest_GP_Map::CreateContainer(UPaperTileLayer& layer, FIntPoint& coordinates)
{
    SpawnBPTile(Chest, 1, coordinates.X, coordinates.Y);

    if (AContainer* newContainer = Cast<AContainer>(FindInteractibleAtGridPosition(coordinates.X, coordinates.Y)))
    {
        // 5) Tirer au hasard un objet parmi la liste RessourcesQuantity ayant une quantité > 0 
        int IDObject = 0;

        while (GameManager->RessourcesQuantity[IDObject] == 0)
        {
            IDObject = (IDObject + 1) % GameManager->RessourcesQuantity.Num();
        }

        if (newContainer->RessourceInside.Num() == 0 || newContainer->RessourceInside[0] == nullptr)
        {
            // 6) Assigner un objet de la liste RessourcesType dans le coffre créé
            newContainer->RessourceInside.Add(RessourcesType[IDObject]);

            // 7) Mettre à jour la quantité de cette objet dans la liste RessourcesQuantity
            GameManager->RessourcesQuantity[IDObject]--;
        }
    }
}

/// <summary>
/// Put trigger ending on map
/// </summary>
/// <param name="layer"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void ATest_GP_Map::CreateTriggerEnding(UPaperTileLayer& layer)
{
    std::unique_ptr<FIntPoint> EndingCoordinates = std::make_unique<FIntPoint>(Stream.RandRange(0, 1) == 0 ? 0 : GridWidth - 1, Stream.RandRange(0, GridHeight - 1));

    bool coordinatesIsGood = false;
    int firstYValue = EndingCoordinates->Y - 1;

    while (!coordinatesIsGood)
    {
        // Check if it's possible to pu Ending trigger on coordinates
        if (EndingCoordinates->Y != firstYValue &&
            !IsTileUserDataEqual(layer, EndingCoordinates->X, EndingCoordinates->Y + 1, TEXT("GROUND")) ||
            FindInteractibleAtGridPosition(EndingCoordinates->X, EndingCoordinates->Y) ||
            FindInteractibleAtGridPosition(EndingCoordinates->X + 1, EndingCoordinates->Y) || FindInteractibleAtGridPosition(EndingCoordinates->X + 2, EndingCoordinates->Y) ||
            FindInteractibleAtGridPosition(EndingCoordinates->X - 1, EndingCoordinates->Y) || FindInteractibleAtGridPosition(EndingCoordinates->X - 2, EndingCoordinates->Y))
        {
            EndingCoordinates->Y = (EndingCoordinates->Y + 1) % GridHeight;
        }
        // If all y values was check with this x, change x value
        else if (EndingCoordinates->Y == firstYValue)
        {
            EndingCoordinates->X = EndingCoordinates->X == 0 ? GridWidth - 1 : 0;
        }
        // Coordinates are good !
        else
        {
            coordinatesIsGood = true;

            // Put trigger ending on ground
            EndingCoordinates->Y++;
        }
    }

    SpawnBPTile(EndingTrigger, 3, EndingCoordinates->X, EndingCoordinates->Y);
    ATriggerEnding* triggerEndingBP = Cast<ATriggerEnding>(FindInteractibleAtGridPosition(EndingCoordinates->X, EndingCoordinates->Y));

    if (EndingCoordinates->X == 0 && triggerEndingBP)
    {
        triggerEndingBP->isOnMapLeftSide = true;
    }
}

/// <summary>
/// Destroy first instance of player create by Game mode
/// </summary>
void ATest_GP_Map::DeleteFirstPlayerInstance()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController) return;

    APawn* PlayerPawn = PlayerController->GetPawn();
    if (PlayerPawn)
    {
        PlayerPawn->Destroy();
    }
}

/// <summary>
/// 
/// </summary>
/// <param name="layer"></param>
void ATest_GP_Map::SpawnPlayerInScene(UPaperTileLayer& layer)
{
    std::unique_ptr<FIntPoint> StartingCoordinates = std::make_unique<FIntPoint>(Stream.RandRange(2, GridHeight - 2), Stream.RandRange(2, GridWidth - 2));
    bool coordinatesIsGood = false;

    while (!coordinatesIsGood)
    {
        if (!IsTileUserDataEqual(layer, StartingCoordinates->X, StartingCoordinates->Y + 1, TEXT("GROUND")))
        {
            StartingCoordinates->Y = (StartingCoordinates->Y + 1) % GridHeight;
        }
        else
        {
            coordinatesIsGood = true;
        }
    }

    FActorSpawnParameters SpawnParams;
    APlayerCharacter* NewPlayer = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharacter, ConvertGridPositionToWorldPosition(StartingCoordinates->X, StartingCoordinates->Y, true), FRotator::ZeroRotator, SpawnParams);

    if (NewPlayer)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        PlayerController->Possess(NewPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Échec du respawn du joueur"));
    }

    //TODO : Delete BP_Player2
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
