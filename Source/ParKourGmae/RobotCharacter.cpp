// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotCharacter.h"
#include "GameFramework/SpringArmComponent.h" //���ɱ�
#include "Camera/CameraComponent.h"//���
#include "Components/InputComponent.h"//�������
#include "GameFramework/CharacterMovementComponent.h"//�ƶ����
#include "GameFramework/Controller.h"
#include "ParKourGmaeGameModeBase.h" //�����ɫ����Ϸģʽͷ�ļ�
// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�������ɱ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//�����ɱ������������
	CameraBoom->SetupAttachment(RootComponent);
	//���õ��ɱ۳���
	CameraBoom->TargetArmLength = 400.0f;
	//���õ��ɱ��ܵ���������ת����
	CameraBoom->bUsePawnControlRotation = true;
	//����������
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//��������ص����ɱ������ĩ��
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//����������ܵ�����������ת����
	FollowCamera->bUsePawnControlRotation = false;
	//���ý�ɫ��Ծ�߶�
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	
}

// Called when the game starts or when spawned
void ARobotCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(1);
	Turn();

}

// Called to bind functionality to input
void ARobotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���PlayerInputComponent�Ƿ���Ч
	check(PlayerInputComponent);
	//����Ծ�����󶨵���Ծ�¼���
	PlayerInputComponent->BindAction("MoveJump", IE_Pressed, this, &ACharacter::Jump);
	//��ֹͣ��Ծ�����󶨵���Ծ�¼���
	PlayerInputComponent->BindAction("MoveJump", IE_Released, this, &ACharacter::StopJumping);

	//����ת
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ARobotCharacter::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ARobotCharacter::TurnLeft);

	//�������ƶ������󶨵���ӳ��MoveRight
	PlayerInputComponent->BindAxis("MoveRight", this, &ARobotCharacter::MoveRight);
}


void ARobotCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		//��ȡ��ҿ���������ת
		const FRotator ControllerRotation = Controller->GetControlRotation();
		//����һ��ֻ����Yaw�ᣨZ������ת
		const FRotator ControllerRotationYaw = FRotator(0, ControllerRotation.Yaw, 0);
		//��ȡ��������ǰ���ĵ�λ����
		const FVector Direction = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::X);
		//������ǰ���ƶ�
		AddMovementInput(Direction, Value);
		//���+1
		AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());;
		if (ParkourGameMode != NULL) ParkourGameMode->MileNum++;
	}
}

void ARobotCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		//��ȡ��ҿ���������ת
		const FRotator ControllerRotation = Controller->GetControlRotation();
		//����һ��ֻ����Yaw�ᣨZ������ת
		const FRotator ControllerRotationYaw = FRotator(0, ControllerRotation.Yaw, 0);
		//��ȡ���������ҷ��ĵ�λ����
		const FVector Direction = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::Y);
		//�������ҷ��ƶ�
		AddMovementInput(Direction, Value);
	}
}

void ARobotCharacter::Turn()
{

	Controller->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DemandRotation, GetWorld()->GetRealTimeSeconds(),0.02)); //ת��������,0.05��ת���ٶ�

}

void ARobotCharacter::TurnRight()
{
	if (IsCanTurn) 
	{
		DemandRotation = GetControlRotation() + FRotator(0, 90, 0);
		IsCanTurn = false;
	}

}

void ARobotCharacter::TurnLeft()
{
	if (IsCanTurn)
	{
		DemandRotation = GetControlRotation() + FRotator(0, -90, 0);
		IsCanTurn = false;
	}
}
