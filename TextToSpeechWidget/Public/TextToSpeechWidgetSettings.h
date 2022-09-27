// CopyRight(c)2019 51VR All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TextToSpeechWidgetSettings.generated.h"

UCLASS(BlueprintType, config = TextToSpeechWidget, DefaultConfig)
class UTextToSpeechWidgetSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UTextToSpeechWidgetSettings();
	
	static UTextToSpeechWidgetSettings* Get();

	virtual FName GetCategoryName() const override { return FName("SpeechStudio"); }

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "TextToSpeech")
	TArray<FString> VoiceArray;
	
};



