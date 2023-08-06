// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "RobotCharacter.h"
#include "ParKourGmaeGameModeBase.h" //�����ɫ����Ϸģʽͷ�ļ�
#include "Math/UnrealMathUtility.h"
// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�������
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	TurnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TurnBox"));
	FloorArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FloorArrow"));
	CoinArrowA = CreateDefaultSubobject<UArrowComponent>(TEXT("CoinArrowA"));
	CoinArrowB = CreateDefaultSubobject<UArrowComponent>(TEXT("CoinArrowB"));

	//��������Ĳ㼶��ϵ
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
	Box->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloor::BoxOverlap); //Ϊ��ײ������ص�����
	TurnBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFloor::TurnBoxOverlap); //Ϊ��ת������ص�����
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
	//��ȡ��ͷλ��
	NextTransform.SetLocation(FloorArrow->GetComponentToWorld().GetLocation());
	//��ȡ��ͷ��ת
	NextTransform.SetRotation(FloorArrow->GetComponentQuat());

	return NextTransform;
}

void AFloor::BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ARobotCharacter::StaticClass()) && GetWorld() != NULL)
	{
		//��ȡ��Ϸģʽ
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
	//�жϵ�ǰActor�Ƿ��ǽ�ɫActor
	if (OtherActor->IsA(ARobotCharacter::StaticClass())) 
	{
		if(IsOnce){
		
			((ARobotCharacter*)OtherActor)->IsCanTurn = true; //OtherActorǿתΪ��ɫActor
			IsOnce = false;
		}
		
	}
}

void AFloor::DestroyFloor()
{
	//�������ý��
	for (int32 i = 0; i < FloorCoins.Num(); i++) 
	{
		if (FloorCoins[i] != NULL)
		{
			FloorCoins[i]->Destroy();
		}
	}
	//���ٵذ�
	this->Destroy();
}

void AFloor::SpawnCoin(UArrowComponent* CoinArrow)
{
	FloorCoins.Empty();
	//��ȡ��Ϸģʽ
	AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());

	if ((ParkourGameMode !=NULL) && (ParkourGameMode->BP_Coin != NULL) && GetWorld())
	{
		//��ȡ��ҵ�λ�ú���ת
		FVector Coinposition = CoinArrow->GetComponentLocation();
		FRotator CoinRotation = CoinArrow->GetComponentRotation();
		//������ɽ��
		int32 CoinNum = FMath::RandRange(2, 8);
		for (int32 i = 0; i < CoinNum; i++) 
		{
			Coinposition = Coinposition + CoinArrow->GetForwardVector() * 150;// �ڼ�ͷ����ǰ������
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
			//������
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
