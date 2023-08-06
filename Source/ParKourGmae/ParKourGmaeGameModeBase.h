// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ParKourGmaeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURGMAE_API AParKourGmaeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AParKourGmaeGameModeBase();

	virtual void BeginPlay() override;
	
	//随机生成地板
	TSubclassOf<AActor> RandomFloor();

	//用来添加地板
	void AddFloor();
	//接受直板资产数组
	TArray<TSubclassOf<AActor>> BP_Floor_ArrayLF; //左右板，uec++中TArray<类型>创建数组
	TArray<TSubclassOf<AActor>> BP_Floor_ArrayF;//直板
	TSubclassOf<AActor> BP_Floor;
	//接受金币资产
	TSubclassOf<AActor> BP_Coin;
	//生成地板位置
	FTransform FloorTransform;
	//保存金币数量
	UPROPERTY(BlueprintReadOnly)//使暴露为蓝图属性
	int32 CoinNum = 0;
	//保存里程
	UPROPERTY(BlueprintReadOnly)//使暴露为蓝图属性
	int32  MileNum = 0;
};
