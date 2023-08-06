// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "RobotCharacter.h"
#include "ParKourGmaeGameModeBase.h" //�����ɫ����Ϸģʽͷ�ļ�
#include "Kismet/GameplayStatics.h" //��������
// Sets default values
ACoin::ACoin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������̬���������
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	//������ײ�����
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));

	//���ò㼶
	Box->SetupAttachment(RootComponent);
	CoinMesh->SetupAttachment(Box);

}
// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	//����ص��¼�
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
	//�ж��Ƿ������
	if (OtherActor->IsA(ARobotCharacter::StaticClass()) && ParkourGameMode !=NULL)
	{
		//ʹ�������+1
		ParkourGameMode->CoinNum++;
		//������Ч
		if (CoinSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, CoinSound, GetActorLocation());
		}
		//���ٵ�ǰ���
		this->Destroy();
		
	}
}
