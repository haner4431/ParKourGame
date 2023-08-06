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
	
	//������ɵذ�
	TSubclassOf<AActor> RandomFloor();

	//������ӵذ�
	void AddFloor();
	//����ֱ���ʲ�����
	TArray<TSubclassOf<AActor>> BP_Floor_ArrayLF; //���Ұ壬uec++��TArray<����>��������
	TArray<TSubclassOf<AActor>> BP_Floor_ArrayF;//ֱ��
	TSubclassOf<AActor> BP_Floor;
	//���ܽ���ʲ�
	TSubclassOf<AActor> BP_Coin;
	//���ɵذ�λ��
	FTransform FloorTransform;
	//����������
	UPROPERTY(BlueprintReadOnly)//ʹ��¶Ϊ��ͼ����
	int32 CoinNum = 0;
	//�������
	UPROPERTY(BlueprintReadOnly)//ʹ��¶Ϊ��ͼ����
	int32  MileNum = 0;
};
