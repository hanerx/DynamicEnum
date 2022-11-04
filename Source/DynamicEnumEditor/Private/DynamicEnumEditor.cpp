#include "DynamicEnumEditor.h"

#include "DynamicEnumBase.h"
#include "Customization/DynamicEnumPropertyTypeCustomization.h"

#define LOCTEXT_NAMESPACE "FDynamicEnumEditorModule"

void FDynamicEnumEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FDynamicEnum::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDynamicEnumPropertyTypeCustomization::MakeInstance));
}

void FDynamicEnumEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDynamicEnumEditorModule, DynamicEnumEditor)