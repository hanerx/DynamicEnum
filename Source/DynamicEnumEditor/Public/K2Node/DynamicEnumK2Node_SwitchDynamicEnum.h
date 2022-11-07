// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicEnumBase.h"
#include "K2Node_Switch.h"
#include "UObject/Object.h"
#include "DynamicEnumK2Node_SwitchDynamicEnum.generated.h"

class FBlueprintActionDatabaseRegistrar;

/**
 * 
 */
UCLASS()
class DYNAMICENUMEDITOR_API UDynamicEnumK2Node_SwitchDynamicEnum : public UK2Node_Switch
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = PinOptions)
	TArray<FDynamicEnum> PinOptions;
	
	UPROPERTY()
	TArray<FName> PinNames;
	
public:
	// UEdGraphNode interface
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool ShouldShowNodeProperties() const override { return true; }
	// End of UEdGraphNode interface

public:
	// UObject interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostLoad() override;
	// End of UObject interface

public:
	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of UK2Node interface

	// UK2Node_Switch Interface
	virtual void AddPinToSwitchNode() override;
	virtual FName GetUniquePinName() override;
	virtual FEdGraphPinType GetPinType() const override;
	virtual FEdGraphPinType GetInnerCaseType() const override;
	// End of UK2Node_Switch Interface

	virtual FName GetPinNameGivenIndex(int32 Index) const override;

protected:
	virtual void CreateFunctionPin() override;
	virtual void CreateSelectionPin() override;
	virtual void CreateCasePins() override;
	virtual void RemovePin(UEdGraphPin* TargetPin) override;
};
