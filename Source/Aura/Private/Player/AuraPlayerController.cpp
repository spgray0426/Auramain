// Copyright Druid Mechanics

/*
 * AuraPlayerController.cpp
 * 플레이어 컨트롤러 구현
 *
 * 탑다운 ARPG 스타일의 플레이어 입력 처리:
 * - 마우스 커서 트레이스 및 하이라이트
 * - 키보드 WASD 이동
 * - 어빌리티 입력 처리
 * - 매직 서클 표시
 * - 데미지 숫자 표시
 */

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Input/AuraInputComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/HighlightInterface.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

/**
 * 커서 아래 액터 트레이스
 * 하이라이트 인터페이스를 구현한 액터를 감지하고 하이라이트 처리
 */
void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())

		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

/** 어빌리티 입력 키 눌림 */
void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

/** 어빌리티 입력 키 해제 */
void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

/** 어빌리티 입력 키 홀드 */
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

/** WASD 키보드 이동 */
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

// === 치트 콘솔 명령어 구현 ===

void AAuraPlayerController::SetLevel(int32 InLevel)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->SetLevel(FMath::Max(1, InLevel));

		// 어빌리티 상태 업데이트 (새 레벨에 맞는 스킬 해금)
		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AuraPS->GetAbilitySystemComponent()))
		{
			AuraASC->UpdateAbilityStatuses(InLevel);
		}

		UE_LOG(LogTemp, Warning, TEXT("Level set to: %d"), InLevel);
	}
}

void AAuraPlayerController::AddLevel(int32 InLevel)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		const int32 NewLevel = AuraPS->GetPlayerLevel() + InLevel;
		SetLevel(NewLevel);
	}
}

void AAuraPlayerController::SetSpellPoints(int32 InPoints)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->SetSpellPoints(FMath::Max(0, InPoints));
		UE_LOG(LogTemp, Warning, TEXT("SpellPoints set to: %d"), InPoints);
	}
}

void AAuraPlayerController::AddSpellPoints(int32 InPoints)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToSpellPoints(InPoints);
		UE_LOG(LogTemp, Warning, TEXT("SpellPoints: %d"), AuraPS->GetSpellPoints());
	}
}

void AAuraPlayerController::SetAttributePoints(int32 InPoints)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->SetAttributePoints(FMath::Max(0, InPoints));
		UE_LOG(LogTemp, Warning, TEXT("AttributePoints set to: %d"), InPoints);
	}
}

void AAuraPlayerController::AddAttributePoints(int32 InPoints)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToAttributePoints(InPoints);
		UE_LOG(LogTemp, Warning, TEXT("AttributePoints: %d"), AuraPS->GetAttributePoints());
	}
}

void AAuraPlayerController::SetXP(int32 InXP)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->SetXP(FMath::Max(0, InXP));
		UE_LOG(LogTemp, Warning, TEXT("XP set to: %d"), InXP);
	}
}

void AAuraPlayerController::AddXP(int32 InXP)
{
	if (AAuraPlayerState* AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToXP(InXP);
		UE_LOG(LogTemp, Warning, TEXT("XP: %d"), AuraPS->GetXP());
	}
}
