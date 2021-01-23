#include "ManualPopulation.h"
#include "TileGroup.h"
#include "Tile.h"
#include <vector>

TileGroup ManualPopulation::CreateNewToDisneyTileGroup()
{
    Tile* t1 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/3C33485A3043C22B8C89E131693E8B5B9306DAA4E48612A655560752977728A6/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500","The Right Stuff");

    Tile* t2 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/E7BFFF8CD4E7BA85BFB3439CDF90698213E8134E4CC729A9AFA17A2E1FC665D31/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "The Mandolorian");

    Tile* t3 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/D9E15BEBF02DF90BD15B68EE79BDBD1385CDEBE7C7B0F7F2D8EB55AF1C247918/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Prep & Landing");

    Tile* t4 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/BC0FC8A0E66457DC34A80EB428F5C91997F2F9BEEAB4AE6A4A6B43BEE4A2726D/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "The Magic of Disneys Animal Kingdom");

    Tile* t5 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/F47E8E48A9541AD7F4BBB19C2F3DBA404A84A6B424AC1188DA7075B50C1E8529/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Clouds");

    Tile* t6 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/CC19113C1DD83A90981AE0B0D8462DD4F8520E204046C577C09E30159A581F24/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Prep & Landing: Naughty vs. Nice - movie");

    Tile* t7 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/3F07134B0065C4422FF242D9E05B4F928C2AF095B80258EFE2385C855E3201E0/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "The Big Fib");

    Tile* t8 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/2B884534715FA1579511A5D4AAAB67753BD8040E091E82E75A10861BCF20A5A0/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "The Sorcerer's Apprentice");

    Tile* t9 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/7C07BD09B722805D8B9B5E8310107758FED69DB57DB83E16FF41B166A2C2B408/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Disney's a Christmas Carol");

    Tile* t10 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/855428541F1F3F96C86DDDF6D49AB6B474BE97F2C6D49B3021A74201247A6932/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "The Simpsons");

    Tile* t11 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/FE5663037ECD6471CB1C3F9D394009396BBEAB4010D89273058AEEFB7C9709B8/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Mr Magoo");

    Tile* t12 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/11A45865D4CB53612EC3C26C26C5865149C83E87555FA1C260CF90A479C543F2/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Once Upon a Snowman");

    Tile* t13 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/0E5C725BBA3EA99D88A9CFE3F153FFF46A497CE5E2004661B472AFDF2AE84026/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "Gathering Storm");

    Tile* t14 = new Tile("https://prod-ripcut-delivery.disney-plus.net/v1/variant/disney/1AC59DDB2B2F1CDE22C159B067153E6074B0BF0EAECE847E484B3ADDDD0F0C9C/scale?format=jpeg&quality=90&scalingAlgorithm=lanczos3&width=500", "One Day at Disney Shorts");

   std::vector<Tile> tiles;
   tiles.push_back(*t1);
   tiles.push_back(*t2);
   tiles.push_back(*t3);
   tiles.push_back(*t4);
   tiles.push_back(*t5);
   tiles.push_back(*t6);
   tiles.push_back(*t7);
   tiles.push_back(*t8);
   tiles.push_back(*t9);
   tiles.push_back(*t10);
   tiles.push_back(*t11);
   tiles.push_back(*t12);
   tiles.push_back(*t13);
   tiles.push_back(*t14);

   TileGroup group(tiles,"New at Disney +");

   return group;
}
void ManualPopulation::CreateCollectionsTileGroup()
{

}
void ManualPopulation::CreateInspiredByTrueStoriesTileGroup()
{

}
void ManualPopulation::CreateDocumentariesTileGroup()
{

}
void ManualPopulation::CreateBecauseYouWatchedGordanRamseyTileGroup()
{

}
void ManualPopulation::CreateTrendingTileGroup()
{

}
void ManualPopulation::CreateReimaginedClassicsTileGroup()
{

}
void ManualPopulation::CreateNostalgicMoviesTileGroup()
{

}
void ManualPopulation::CreateUltraHDTileGroup()
{

}
void ManualPopulation::CreateAnimalsAndNatiureTileGroup()
{

}
void ManualPopulation::CreateFeelGoodSportsTileGroup()
{

}
void ManualPopulation::CreateAnimatedMoviesTileGroup()
{

}
void ManualPopulation::CreateMickeyAndFriendsTileGroup()
{

}