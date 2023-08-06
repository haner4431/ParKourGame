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

	//声明一个场景组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	//声明一个静态网格体组件,地板模型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* FloorMesh;
	//声明Box组件,用来碰撞检测
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Box;
	//声明Box组件,用来旋转检测
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* TurnBox;

	//声明Arrow组件,用来记录下一个地板生成位置
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* FloorArrow;
	//金币生成的方向
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* CoinArrowA;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* CoinArrowB;

	//返回下一个地板生成的位置
	FTransform GetNextFloorTransform();

	//绑定碰撞重叠函数
	UFUNCTION()
		void BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//绑定旋转重叠函数
	UFUNCTION()
		void TurnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//销毁用的定时器
	FTimerHandle DestroyTimerHandle;

	//定时器执行的函数
	void DestroyFloor();
	//用于控制只触发一次重叠
	bool IsOnce = true;

	//存储当前地板的所有金币
	TArray<AActor*> FloorCoins;
	//生产金币
	void SpawnCoin(class UArrowComponent* CoinArrow);
	//随机箭头组件
	void RandomArrowSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFloorType FloorType = EFloorType::StraightFloor;
};
