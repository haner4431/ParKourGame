// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UENUM(BlueprintType)
enum class EFloorType : uint8
{
	StraightFloor,
	TurnFLoor
};


UCLASS()
class PARKOURGMAE_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//����һ���������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	//����һ����̬���������,�ذ�ģ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FloorMesh;
	//����Box���,������ײ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;
	//����Box���,������ת���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* TurnBox;

	//����Arrow���,������¼��һ���ذ�����λ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* FloorArrow;
	//������ɵķ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* CoinArrowA;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* CoinArrowB;

	//������һ���ذ����ɵ�λ��
	FTransform GetNextFloorTransform();

	//����ײ�ص�����
	UFUNCTION()
		void BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//����ת�ص�����
	UFUNCTION()
		void TurnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�����õĶ�ʱ��
	FTimerHandle DestroyTimerHandle;

	//��ʱ��ִ�еĺ���
	void DestroyFloor();
	//���ڿ���ֻ����һ���ص�
	bool IsOnce = true;

	//�洢��ǰ�ذ�����н��
	TArray<AActor*> FloorCoins;
	//�������
	void SpawnCoin(class UArrowComponent* CoinArrow);
	//�����ͷ���
	void RandomArrowSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloorType FloorType = EFloorType::StraightFloor;
};
