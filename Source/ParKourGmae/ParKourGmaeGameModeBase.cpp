// Copyright Epic Games, Inc. All Rights Reserved.


#include "ParKourGmaeGameModeBase.h"
#include "Floor.h"
#include "Math/UnrealMathUtility.h"
AParKourGmaeGameModeBase::AParKourGmaeGameModeBase() {
	//清空数组
	BP_Floor_ArrayLF.Empty();
	BP_Floor_ArrayF.Empty();
	//添加地板资产，加载蓝图资产需要在引用后面加_C
	TSubclassOf<AActor> BP_Floor1 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor.BP_Floor_C'"));
	BP_Floor_ArrayF.Add(BP_Floor1);
	TSubclassOf<AActor> BP_Floor2 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_2.BP_Floor_2_C'"));
	BP_Floor_ArrayF.Add(BP_Floor2);
	TSubclassOf<AActor> BP_Floor3 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_3.BP_Floor_3_C'"));
	BP_Floor_ArrayF.Add(BP_Floor3);
	TSubclassOf<AActor> BP_Floor4 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_4.BP_Floor_4_C'"));
	BP_Floor_ArrayF.Add(BP_Floor4);
	TSubclassOf<AActor> BP_Floor5 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_5.BP_Floor_5_C'"));
	BP_Floor_ArrayLF.Add(BP_Floor5);//uec++向数组添加元素
	TSubclassOf<AActor> BP_Floor6 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_6.BP_Floor_6_C'"));
	BP_Floor_ArrayLF.Add(BP_Floor6);

	//上下板
	TSubclassOf<AActor> BP_Floor7 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_7.BP_Floor_7_C'"));
	BP_Floor_ArrayF.Add(BP_Floor7);
	TSubclassOf<AActor> BP_Floor8 = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BpFloor/BP_Floor_8.BP_Floor_8_C'"));
	BP_Floor_ArrayF.Add(BP_Floor8);

	//加载金币资产
	BP_Coin = LoadClass<AActor>(NULL, TEXT("Blueprint'/Game/ParkourGame/BluePrints/BP_Coin.BP_Coin_C'"));


}

void AParKourGmaeGameModeBase::BeginPlay()
{
	Super::BeginPlay();


	if (GetWorld())
	{
		for (int32 i = 0; i < 10; i++)
		{
			AddFloor(); //游戏初始生成10个地板
		}
	}

}

TSubclassOf<AActor> AParKourGmaeGameModeBase::RandomFloor()
{
	int32 i = FMath::RandRange(1,100);
	if (i <= 90) 
	{
		int32 j = FMath::RandRange(0, BP_Floor_ArrayF.Num()-1);
		return BP_Floor_ArrayF[j];
	}
	else
	{
		int32 k = FMath::RandRange(0, BP_Floor_ArrayLF.Num() - 1);
		return BP_Floor_ArrayLF[k];
	}
}

void AParKourGmaeGameModeBase::AddFloor()
{
	BP_Floor = RandomFloor();
	if (BP_Floor != NULL && GetWorld() != NULL)
	{
		//生成蓝图模板类实例，用父类存储
		AFloor* CurrentFloor = GetWorld()->SpawnActor<AFloor>(BP_Floor, FloorTransform);
		FloorTransform = CurrentFloor->GetNextFloorTransform();
	}
}
