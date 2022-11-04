// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DynamicEnumBase.generated.h"

USTRUCT(BlueprintType)
struct DYNAMICENUM_API FDynamicEnum
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="DynamicEnum")
	UEnum* EnumType;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="DynamicEnum")
	int32 Value;

public:
	bool IsValid() const { return EnumType != nullptr && Value >= 0 && Value < EnumType->NumEnums()-1; }

	FString ToString() const { return IsValid() ? EnumType->GetNameStringByIndex(Value) : TEXT("Invalid"); }

	FName ToName() const { return IsValid() ? EnumType->GetNameByIndex(Value) : TEXT("Invalid"); }

	bool operator == (const FDynamicEnum& Other) const { return EnumType == Other.EnumType && Value == Other.Value; }

	bool operator != (const FDynamicEnum& Other) const { return !(*this == Other); }

	
};

UCLASS()
class DYNAMICENUM_API UDynamicEnumLibrary_K2 final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="DynamicEnum", meta=(DisplayName="Is Valid (DynamicEnum)",ScriptMethod="IsValid", CompactNodeTitle="isValid?", BlueprintThreadSafe))
	static bool K2_IsValid(const FDynamicEnum& DynamicEnum) { return DynamicEnum.IsValid(); }

	UFUNCTION(BlueprintPure, Category="DynamicEnum", meta=(DisplayName="To String (DynamicEnum)", CompactNodeTitle="->", ScriptMethod="ToString", BlueprintThreadSafe))
	static FString K2_ToString(const FDynamicEnum& DynamicEnum) { return DynamicEnum.ToString(); }

	UFUNCTION(BlueprintPure,Category="DynamicEnum",meta=(DisplayName="To Name (DynamicEnum)", CompactNodeTitle="->", ScriptMethod="ToName", BlueprintThreadSafe))
	static FName K2_ToName(const FDynamicEnum& DynamicEnum) { return DynamicEnum.ToName(); }

	UFUNCTION(BlueprintPure,Category="DynamicEnum",meta=(DisplayName="Equal (DynamicEnum)", CompactNodeTitle="==", BlueprintThreadSafe))
	static bool K2_Equal(const FDynamicEnum& A, const FDynamicEnum& B) { return A == B; }

	UFUNCTION(BlueprintPure,Category="DynamicEnum",meta=(DisplayName="Not Equal (DynamicEnum)", CompactNodeTitle="!=", BlueprintThreadSafe))
	static bool K2_NotEqual(const FDynamicEnum& A, const FDynamicEnum& B) { return A != B; }
	
};
