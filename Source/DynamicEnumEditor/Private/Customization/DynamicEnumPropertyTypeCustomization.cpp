#include "Customization/DynamicEnumPropertyTypeCustomization.h"

#include "DetailWidgetRow.h"
#include "DynamicEnumBase.h"
#include "IDetailChildrenBuilder.h"
#include "SSearchableComboBox.h"
#define LOCTEXT_NAMESPACE "DynamicEnum"

void FDynamicEnumPropertyTypeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                              IDetailChildrenBuilder& ChildBuilder,
                                                              IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	if(IsTypeEditable(PropertyHandle))
	{
		CustomizeEnumType(PropertyHandle, ChildBuilder, CustomizationUtils);
	}else
	{
		SetEnumType(PropertyHandle);
	}
	CustomizeValue(PropertyHandle, ChildBuilder, CustomizationUtils);
}

void FDynamicEnumPropertyTypeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
                                                            FDetailWidgetRow& HeaderRow,
                                                            IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	];
}

void FDynamicEnumPropertyTypeCustomization::OnEnumTypeChanged(TSharedRef<IPropertyHandle> PropertyHandle)
{
	PropertyHandle->NotifyPreChange();
	ResetEnumValue(PropertyHandle);
	GenerateComboOptions(PropertyHandle, ComboOptions);
	PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
	if(ComboBox.IsValid())
	{
		ComboBox->RefreshOptions();
	}
}

void FDynamicEnumPropertyTypeCustomization::CustomizeEnumType(TSharedRef<IPropertyHandle> PropertyHandle,
                                                              IDetailChildrenBuilder& ChildBuilder,
                                                              IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> EnumTypeHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, EnumType));
	ChildBuilder.AddProperty(EnumTypeHandle.ToSharedRef());
	const FSimpleDelegate OnEnumTypeChangedDelegate = FSimpleDelegate::CreateSP(
		this, &FDynamicEnumPropertyTypeCustomization::OnEnumTypeChanged, PropertyHandle);
	EnumTypeHandle->SetOnPropertyValueChanged(OnEnumTypeChangedDelegate);
}

FText FDynamicEnumPropertyTypeCustomization::GetSelectedText(TSharedRef<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> EnumValueHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, Value));
	const TSharedPtr<IPropertyHandle> EnumTypeHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, EnumType));

	if(EnumValueHandle.IsValid() && EnumTypeHandle.IsValid())
	{
		int32 Value;
		EnumValueHandle->GetValue(Value);
		UObject* EnumType;
		EnumTypeHandle->GetValue(EnumType);
		if(const UEnum* Enum = Cast<UEnum>(EnumType))
		{
			return Enum->GetDisplayNameTextByIndex(Value);
		}
	}
	return FText::GetEmpty();
}

TSharedRef<SWidget> FDynamicEnumPropertyTypeCustomization::OnGenerateWidget(TSharedPtr<FString> String) const
{
	return SNew(STextBlock).Text(FText::FromString(*String));
}

void FDynamicEnumPropertyTypeCustomization::OnSelectionChanged(TSharedPtr<FString> String, ESelectInfo::Type Arg,
	TSharedRef<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> EnumValueHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, Value));
	if(ComboOptions.Contains(String))
	{
		EnumValueHandle->SetValue(ComboOptions.Find(String));
	}
}

void FDynamicEnumPropertyTypeCustomization::CustomizeValue(TSharedRef<IPropertyHandle> PropertyHandle,
                                                           IDetailChildrenBuilder& ChildBuilder,
                                                           IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> EnumValueHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, Value));


	GenerateComboOptions(PropertyHandle, ComboOptions);
	IDetailPropertyRow& EnumValueRow = ChildBuilder.AddProperty(EnumValueHandle.ToSharedRef());
	EnumValueRow.CustomWidget(true)
	            .EditCondition(IsValueEditable(PropertyHandle), nullptr)
	            .NameContent()
		[
			EnumValueHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SAssignNew(ComboBox,SSearchableComboBox)
			.Content()
			[
				SNew(STextBlock).Text(this,&FDynamicEnumPropertyTypeCustomization::GetSelectedText,PropertyHandle)
			]
			.OptionsSource(&ComboOptions)
			.OnGenerateWidget(this,&FDynamicEnumPropertyTypeCustomization::OnGenerateWidget)
			.OnSelectionChanged(this,&FDynamicEnumPropertyTypeCustomization::OnSelectionChanged,PropertyHandle)
			.IsEnabled(this,&FDynamicEnumPropertyTypeCustomization::IsValueEditable,PropertyHandle)
		];
}

bool FDynamicEnumPropertyTypeCustomization::IsValueEditable(TSharedRef<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> EnumTypeHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, EnumType));
	UObject* ObjectValue;
	if (EnumTypeHandle->GetValue(ObjectValue) == FPropertyAccess::Result::Success && Cast<UEnum>(ObjectValue))
	{
		return true;
	}
	return false;
}

bool FDynamicEnumPropertyTypeCustomization::IsTypeEditable(TSharedRef<IPropertyHandle> PropertyHandle) const
{
	if(PropertyHandle->HasMetaData(TEXT("Enum")))
	{
		if(FindObject<UEnum>(ANY_PACKAGE,*PropertyHandle->GetMetaData(TEXT("Enum"))))
		{
			return false;
		}
	}
	return true;
}

void FDynamicEnumPropertyTypeCustomization::GenerateComboOptions(TSharedRef<IPropertyHandle> PropertyHandle,
                                                                 TArray<TSharedPtr<FString>>& OutOptions) const
{
	const TSharedPtr<IPropertyHandle> EnumTypeHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, EnumType));
	UObject* ObjectValue;
	if (EnumTypeHandle->GetValue(ObjectValue) == FPropertyAccess::Result::Success)
	{
		if (const UEnum* Enum = Cast<UEnum>(ObjectValue))
		{
			for (int32 i = 0; i < Enum->NumEnums()-1; i++)
			{
				const bool bShouldBeHidden = Enum->HasMetaData(TEXT("Hidden"), i) || Enum->HasMetaData(
					TEXT("Spacer"), i)||EnumTypeHandle->IsHidden(Enum->GetNameStringByIndex(i));
				if (!bShouldBeHidden)
				{
					OutOptions.Add(MakeShared<FString>(Enum->GetDisplayNameTextByIndex(i).ToString()));
				}
			}
		}
	}
}

void FDynamicEnumPropertyTypeCustomization::ResetEnumValue(TSharedRef<IPropertyHandle> PropertyHandle)
{
	const TSharedPtr<IPropertyHandle> EnumValueHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, Value));
	EnumValueHandle->SetValue(0);
	ComboOptions.Empty();
}

void FDynamicEnumPropertyTypeCustomization::SetEnumType(TSharedRef<IPropertyHandle> PropertyHandle) const
{
	const TSharedPtr<IPropertyHandle> EnumTypeHandle = PropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FDynamicEnum, EnumType));
	if(PropertyHandle->HasMetaData(TEXT("Enum")))
	{
		if(UEnum* Enum=FindObject<UEnum>(ANY_PACKAGE,*PropertyHandle->GetMetaData(TEXT("Enum"))))
		{
			PropertyHandle->NotifyPreChange();
			EnumTypeHandle->SetValue(Enum);
			PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
		}
	}
}

#undef LOCTEXT_NAMESPACE
