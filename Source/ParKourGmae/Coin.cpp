// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "RobotCharacter.h"
#include "ParKourGmaeGameModeBase.h" //引入角色和游戏模式头文件
#include "Kismet/GameplayStatics.h" //播放声音
// Sets default values
ACoin::ACoin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建静态网格体组件
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	//创建碰撞框组件
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));

	//设置层级
	Box->SetupAttachment(RootComponent);
	CoinMesh->SetupAttachment(Box);

}
// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	//添加重叠事件
	Box->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACoin::CoinOverlap);
	
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoin::CoinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());
	//判断是否是玩家
	if (OtherActor->IsA(ARobotCharacter::StaticClass()) && ParkourGameMode !=NULL)
	{
		//使金币数量+1
		ParkourGameMode->CoinNum++;
		//播放音效
		if (CoinSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, CoinSound, GetActorLocation());
		}
		//销毁当前金币
		this->Destroy();
		
	}
}
