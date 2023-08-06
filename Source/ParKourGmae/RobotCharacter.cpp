// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotCharacter.h"
#include "GameFramework/SpringArmComponent.h" //弹簧臂
#include "Camera/CameraComponent.h"//相机
#include "Components/InputComponent.h"//输入组件
#include "GameFramework/CharacterMovementComponent.h"//移动组件
#include "GameFramework/Controller.h"
#include "ParKourGmaeGameModeBase.h" //引入角色和游戏模式头文件
// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//创建弹簧臂组件
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//将弹簧臂依附到根组件
	CameraBoom->SetupAttachment(RootComponent);
	//设置弹簧臂长度
	CameraBoom->TargetArmLength = 400.0f;
	//设置弹簧臂受到控制器旋转控制
	CameraBoom->bUsePawnControlRotation = true;
	//创建相机组件
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//将相机挂载到弹簧臂组件的末端
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//设置相机不受到控制器的旋转控制
	FollowCamera->bUsePawnControlRotation = false;
	//设置角色跳跃高度
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

	//检测PlayerInputComponent是否有效
	check(PlayerInputComponent);
	//将跳跃函数绑定到跳跃事件上
	PlayerInputComponent->BindAction("MoveJump", IE_Pressed, this, &ACharacter::Jump);
	//将停止跳跃函数绑定到跳跃事件上
	PlayerInputComponent->BindAction("MoveJump", IE_Released, this, &ACharacter::StopJumping);

	//绑定旋转
	PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &ARobotCharacter::TurnRight);
	PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &ARobotCharacter::TurnLeft);

	//将左右移动函数绑定到轴映射MoveRight
	PlayerInputComponent->BindAxis("MoveRight", this, &ARobotCharacter::MoveRight);
}


void ARobotCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		//获取玩家控制器的旋转
		const FRotator ControllerRotation = Controller->GetControlRotation();
		//制造一个只包含Yaw轴（Z）的旋转
		const FRotator ControllerRotationYaw = FRotator(0, ControllerRotation.Yaw, 0);
		//获取控制器正前方的单位向量
		const FVector Direction = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::X);
		//朝着正前方移动
		AddMovementInput(Direction, Value);
		//里程+1
		AParKourGmaeGameModeBase* ParkourGameMode = Cast<AParKourGmaeGameModeBase>(GetWorld()->GetAuthGameMode());;
		if (ParkourGameMode != NULL) ParkourGameMode->MileNum++;
	}
}

void ARobotCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		//获取玩家控制器的旋转
		const FRotator ControllerRotation = Controller->GetControlRotation();
		//制造一个只包含Yaw轴（Z）的旋转
		const FRotator ControllerRotationYaw = FRotator(0, ControllerRotation.Yaw, 0);
		//获取控制器正右方的单位向量
		const FVector Direction = FRotationMatrix(ControllerRotationYaw).GetUnitAxis(EAxis::Y);
		//朝着左右方移动
		AddMovementInput(Direction, Value);
	}
}

void ARobotCharacter::Turn()
{

	Controller->SetControlRotation(FMath::RInterpTo(GetControlRotation(), DemandRotation, GetWorld()->GetRealTimeSeconds(),0.02)); //转向动作过渡,0.05是转向速度

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
