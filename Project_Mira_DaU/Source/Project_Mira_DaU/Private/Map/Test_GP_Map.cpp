
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "PaperTileLayer.h"
#include "Math/UnrealMathUtility.h"
#include "Map/Test_GP_Map.h"


// ETAPE 1 : Initialiser la TileMapComponent et les valeurs qui seront utilisées
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
                if (IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("GROUND")))
                {
                    CreateBuilding(x, y, BuildingWidth, availableFloorSpace, *NewLayer);
                }
                else
                {   
                    // Construction en Hauteur (on est pas sur le sol)
                    //START (ou GROUND)
                    if (IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("STARTBUILDING")))
                    {
                        // On est au début d'un autre batiment
                        if (CalculHeightValue(*NewLayer, x, y, 0) <= MIN_HEIGHT_BUILDING)
                        {
                            PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, *NewLayer);
                        }
                        else if (CalculHeightValue(*NewLayer, x, y, 0) == MAX_HEIGHT_BUILDING)
                        {
                            PutTileOnGrid(x, y, (int32)ETiles::GROUND, *NewLayer);
                        }
                        else
                        {
                            // probabilité de continuer le bâtiment en hauteur
                            if (BuildBuildingOrNot(PROBA_EXTEND_BUILD_HEIGHT))
                            {
                                PutTileOnGrid(x, y, (int32)ETiles::STARTBUILDING, *NewLayer);
                            }
                            else
                            {
                                PutTileOnGrid(x, y, (int32)ETiles::GROUND, *NewLayer);
                            }
                        }
                    }
                    //GROUND
                    else if (IsTileUserDataEqual(*NewLayer, x - 1, y, TEXT("GROUND")) && (IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("BUILDINGWALL")) || IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("ENDBUILDING"))))
                    {
                        PutTileOnGrid(x, y, (int32)ETiles::GROUND, *NewLayer);
                    }
                    //END
                    else if (IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("ENDBUILDING")))
                    {
                        PutTileOnGrid(x, y, (int32)ETiles::ENDBUILDING, *NewLayer);
                    }
                    //WALL
                    else if (IsTileUserDataEqual(*NewLayer, x, y + 1, TEXT("BUILDINGWALL")))
                    {
                        PutTileOnGrid(x, y, (int32)ETiles::BUILDINGWALL, *NewLayer);
                    }
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
 
/*bool ATest_GP_Map::IsTileNull(UPaperTileLayer& layer, int x, int y) // DOESN'T WORKS
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y);

    return TileInfoCell.PackedTileIndex == INDEX_NONE || TileInfoCell.PackedTileIndex == NULL;
}*/

/*bool ATest_GP_Map::PreviousTileIsAWall(UPaperTileLayer& layer, int x, int y)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x - 1, y);

    if (TileInfoCell.PackedTileIndex != INDEX_NONE && TileInfoCell.PackedTileIndex != NULL)
    {
        // Sur ce layer, les tuiles sont soit des sols, soit des murs, soit vide (et null en dehors de la grille)
        return TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString() != TEXT("Ground");
    }
}*/

/*bool ATest_GP_Map::CompareTwoTilesUserData(UPaperTileLayer& layer, int x1, int y1, int x2, int y2)
{
    FPaperTileInfo TileInfoCell1 = layer.GetCell(x1, y1);
    FPaperTileInfo TileInfoCell2 = layer.GetCell(x2, y2);

    if (TileInfoCell1.PackedTileIndex && TileInfoCell2.PackedTileIndex)
    {
        return TileInfoCell1.TileSet->GetTileUserData(TileInfoCell1.PackedTileIndex).ToString() == TileInfoCell2.TileSet->GetTileUserData(TileInfoCell2.PackedTileIndex).ToString();
    }
    return false;
}*/

bool ATest_GP_Map::BuildBuildingOrNot(int const probability)
{
    return Stream.RandRange(0, 99) < probability;
}

void ATest_GP_Map::CreateBuilding(int const x, int const y, int& width, int& availableFloorSpace, UPaperTileLayer& layer)
{
    if (width == 0)
    {
        availableFloorSpace = CalculWidthValue(layer, x, y, 0);

        // Commencer nouveau batiment : poser une tuile début de bâtiment
        if (availableFloorSpace >= MIN_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_START_BUILDING))
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
               (width < MAX_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_EXTEND_BUILD_WIDTH)))
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

// Eh you là-bas ! Near the betonneuse ! Put the ciment on the poteau please !
void ATest_GP_Map::PutTileOnGrid(int const x, int const y, int32 tile, UPaperTileLayer& layer)
{
    FPaperTileInfo TileInfo;
    TileInfo.TileSet = TileSet;
    TileInfo.PackedTileIndex = tile;

    MyTileMapComponent->SetTile(x, y, layer.GetLayerIndex(), TileInfo);
}

int ATest_GP_Map::CalculHeightValue(UPaperTileLayer& layer, int x, int y, int heightValue = 0)
{
    if (IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")))
    {
        return heightValue;
    }
    else
    {
        heightValue++;
    }

    return CalculHeightValue(layer, x, y + 1, heightValue);
}

int ATest_GP_Map::CalculWidthValue(UPaperTileLayer& layer, int x, int y, int widthValue = 0)
{
    if(!IsTileUserDataEqual(layer, x, y + 1, TEXT("GROUND")))
    {
        return widthValue;
    }
    else
    {
        widthValue++;
    }

    return CalculWidthValue(layer, x + 1, y, widthValue);
}