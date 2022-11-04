#pragma once
#include "SSearchableComboBox.h"

class FDynamicEnumPropertyTypeCustomization : public IPropertyTypeCustomization
{
public:
	FDynamicEnumPropertyTypeCustomization()
	{
	}

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FDynamicEnumPropertyTypeCustomization>();
	}
public:
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	void OnEnumTypeChanged(TSharedRef<IPropertyHandle> PropertyHandle);
	void CustomizeEnumType(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);


	FText GetSelectedText(TSharedRef<IPropertyHandle> PropertyHandle) const;
	TSharedRef<SWidget> OnGenerateWidget(TSharedPtr<FString> String) const;
	void OnSelectionChanged(TSharedPtr<FString> String, ESelectInfo::Type Arg, TSharedRef<IPropertyHandle> PropertyHandle) const;
	void CustomizeValue(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils);

	bool IsValueEditable(TSharedRef<IPropertyHandle> PropertyHandle) const;
	bool IsTypeEditable(TSharedRef<IPropertyHandle> PropertyHandle) const;

	void GenerateComboOptions(TSharedRef<IPropertyHandle> PropertyHandle, TArray<TSharedPtr<FString>>& OutOptions) const;

	void ResetEnumValue(TSharedRef<IPropertyHandle> PropertyHandle);

	void SetEnumType(TSharedRef<IPropertyHandle> PropertyHandle) const;

private:
	TArray<TSharedPtr<FString>> ComboOptions;

	TSharedPtr<SSearchableComboBox> ComboBox;
};
