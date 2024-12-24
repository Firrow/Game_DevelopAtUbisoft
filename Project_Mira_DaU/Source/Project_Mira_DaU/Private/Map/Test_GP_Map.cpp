
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
            UE_LOG(LogTemp, Warning, TEXT("Longueur batiment actuel : %i"), BuildingWidth);

            //Put ground on 
            if (y == GridHeight - 1)
            {
                FPaperTileInfo TileInfo;
                TileInfo.TileSet = TileSet;
                TileInfo.PackedTileIndex = (int32)ETiles::GROUND;

                // Placement de la tuile
                MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);
            }
            else 
            {
                if (CurrentTileIsOnGround(*NewLayer, x, y))
                {
                    if (LeftNeighborIsNull(*NewLayer, x, y))
                    {
                        // (Commencer nouveau batiment)
                        // Tirer au hasard un nombre et comparer avec la constante
                        if (GridWidth - x >= MIN_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_START_BUILDING))
                        {
                            UE_LOG(LogTemp, Warning, TEXT(" Je creer un batiment"));

                            FPaperTileInfo TileInfo;
                            TileInfo.TileSet = TileSet;
                            TileInfo.PackedTileIndex = (int32)ETiles::STARTBUILDING;

                            // Placement de la tuile
                            MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);

                            BuildingWidth = 1;
                        }
                    }
                    else
                    {
                        // Si varLongueurBat <= tailleMin : pauser tuile intérieur
                        if (BuildingWidth <= MIN_WIDTH_BUILDING)
                        {
                            UE_LOG(LogTemp, Warning, TEXT(" Je continue le batiment car taille min pas atteinte"));

                            FPaperTileInfo TileInfo;
                            TileInfo.TileSet = TileSet;
                            if (x == GridWidth - 1)
                            {
                                TileInfo.PackedTileIndex = (int32)ETiles::ENDBUILDING;
                            }
                            else 
                            {
                                TileInfo.PackedTileIndex = (int32)ETiles::BUILDINGWALL;
                            }

                            // Placement de la tuile
                            MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);

                            BuildingWidth++;
                        }
                        else
                        {
                            if (BuildingWidth <= MAX_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_EXTEND_BUILD_WIDTH))
                            {
                                UE_LOG(LogTemp, Warning, TEXT(" Je continue le batiment car hasard"));

                                // pauser tuile intérieur
                                FPaperTileInfo TileInfo;
                                TileInfo.TileSet = TileSet;
                                if (x == GridWidth - 1)
                                {
                                    TileInfo.PackedTileIndex = (int32)ETiles::ENDBUILDING;
                                }
                                else
                                {
                                    TileInfo.PackedTileIndex = (int32)ETiles::BUILDINGWALL;
                                }

                                // Placement de la tuile
                                MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);

                                BuildingWidth++;
                            }
                            else
                            {
                                UE_LOG(LogTemp, Warning, TEXT(" Je stoppe le batiment car hasard"));
                                // Si non : 1) changer tuile précédente par tuile qui ferme batiment
                                //          2) tirer chiffre démarrer nouveau batiment
                                FPaperTileInfo TileInfo;
                                TileInfo.TileSet = TileSet;
                                TileInfo.PackedTileIndex = (int32)ETiles::ENDBUILDING;

                                // Changement de la tuile précédente
                                MyTileMapComponent->SetTile(x - 1, y, NewLayer->GetLayerIndex(), TileInfo);

                                BuildingWidth = 0;

                                if (GridWidth - x >= MIN_WIDTH_BUILDING && BuildBuildingOrNot(PROBA_START_BUILDING))
                                {
                                    UE_LOG(LogTemp, Warning, TEXT(" Je creer un batiment"));

                                    FPaperTileInfo TileInfoNewBuild;
                                    TileInfoNewBuild.TileSet = TileSet;
                                    TileInfoNewBuild.PackedTileIndex = (int32)ETiles::STARTBUILDING;

                                    // Placement de la tuile
                                    MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfoNewBuild);

                                    BuildingWidth = 1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    //QUAND POSER SOL ? (VOIR EN Y)

                    FPaperTileInfo TileInfo;
                    TileInfo.TileSet = TileSet;
                    TileInfo.PackedTileIndex = (int32)ETiles::TEMP;

                    // Placement de la tuile
                    MyTileMapComponent->SetTile(x, y, NewLayer->GetLayerIndex(), TileInfo);
                }
            }

            //UE_LOG(LogTemp, Warning, TEXT("X : %i - Y : %i"), x, y);
        }
    }

    // ETAPE 6 : MAJ des collisions des tuiles
    MyTileMapComponent->RebuildCollision();
}

bool ATest_GP_Map::LeftNeighborIsNull(UPaperTileLayer& layer, int x, int y)
{
    bool res = layer.GetCell(x - 1, y).PackedTileIndex == INDEX_NONE || layer.GetCell(x, y).PackedTileIndex == NULL ? true : false;
    UE_LOG(LogTemp, Warning, TEXT("res : %s"), res == true ? TEXT("true") : TEXT("false"));
    return res;
}

bool ATest_GP_Map::CurrentTileIsOnGround(UPaperTileLayer& layer,  int x, int y)
{
    FPaperTileInfo TileInfoCell = layer.GetCell(x, y + 1);

    if (!TileInfoCell.TileSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("TileSet is null for tile at (%d, %d)"), x, y);
        return false;
    }

    //UE_LOG(LogTemp, Warning, TEXT("UserDataName : %s"), *TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString());

    FString UserData = TileInfoCell.TileSet->GetTileUserData(TileInfoCell.PackedTileIndex).ToString();
    return UserData == TEXT("Ground");
}

bool ATest_GP_Map::BuildBuildingOrNot(int const probability)
{
    int res = FMath::RandRange(0, 99);
    //UE_LOG(LogTemp, Warning, TEXT("res : %i"), res);
    return  res <= probability ? true : false;
}