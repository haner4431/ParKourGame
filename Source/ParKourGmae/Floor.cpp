// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "RobotCharacter.h"
#include "ParKourGmaeGameModeBase.h" //引入角色和游戏模式头文件
#include "Math/UnrealMathUtility.h"
// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//创建组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	TurnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnBox"));
	FloorArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FloorArrow"));
	CoinArrowA = CreateDefaultSubobject<UArrowComponent>(TEXT("CoinArrowA"));
	CoinArrowB = CreateDefaultSubobject<UArrowComponent>(TEXT("CoinArrowB"));

	//设置组件的层级关系
	RootComponent = RootScene;
	FloorMesh->SetupAttachment(RootScene);
	Box->SetupAttachment(RootScene);
	FloorArrow->SetupAttachment(RootScene);
	CoinArrowA->SetupAttachment(RootScene);
	CoinArrowB->SetupAttachment(RootScene);
	TurnBox->SetupAttachment(RootScene);




}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloor::BoxOverlap); //为碰撞盒体绑定重叠函数
	TurnBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloor::TurnBoxOverlap); //为旋转盒体绑定重叠函数
	RandomArrowSpawn();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
FTransform AFloor::GetNextFloorTransform()
{
	FTransform NextTransform;
	//获取箭头位置
	NextTransform.SetLocation(FloorArrow->GetComponentToWorld().GetLocation());
	//获取箭头旋转
	NextTransform.SetRotation(FloorArrow->GetComponentQuat());

	return NextTransform;
}

void AFloor::BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARobotCharacter::StaticClass()) && GetWorld() != NULL)
	{
		//获取游戏模式
		AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());
		if (ParkourGameMode)
		{
			ParkourGameMode->AddFloor();

			if (DestroyTimerHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
			}
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AFloor::DestroyFloor, 2.0f, false);
		}
	}
}

void AFloor::TurnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//判断当前Actor是否是角色Actor
	if (OtherActor->IsA(ARobotCharacter::StaticClass())) 
	{
		if(IsOnce){
		
			((ARobotCharacter*)OtherActor)->IsCanTurn = true; //OtherActor强转为角色Actor
			IsOnce = false;
		}
		
	}
}

void AFloor::DestroyFloor()
{
	//销毁无用金币
	for (int32 i = 0; i < FloorCoins.Num(); i++) 
	{
		if (FloorCoins[i] != NULL)
		{
			FloorCoins[i]->Destroy();
		}
	}
	//销毁地板
	this->Destroy();
}

void AFloor::SpawnCoin(UArrowComponent* CoinArrow)
{
	FloorCoins.Empty();
	//获取游戏模式
	AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());

	if ((ParkourGameMode !=NULL) && (ParkourGameMode->BP_Coin != NULL) && GetWorld())
	{
		//获取金币的位置和旋转
		FVector Coinposition = CoinArrow->GetComponentLocation();
		FRotator CoinRotation = CoinArrow->GetComponentRotation();
		//随机生成金币
		int32 CoinNum = FMath::RandRange(2, 8);
		for (int32 i = 0; i < CoinNum; i++) 
		{
			Coinposition = Coinposition + CoinArrow->GetForwardVector() * 150;// 在箭头的正前方生成
			AActor* Coin = GetWorld()->SpawnActor<AActor>(ParkourGameMode->BP_Coin, Coinposition, CoinRotation);
			FloorCoins.Add(Coin);

		}
	}
}

void AFloor::RandomArrowSpawn()
{
	if (FloorType == EFloorType::StraightFloor)
	{
		int32 i = FMath::RandRange(0, 2);
		switch (i)
		{
		case 0:
			//不生成
			break;
		case 1:
			SpawnCoin(CoinArrowA);
			break;
		case 2:
			SpawnCoin(CoinArrowB);
			break;
		}
	}

	
}
