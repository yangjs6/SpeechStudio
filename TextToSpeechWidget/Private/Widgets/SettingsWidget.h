#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBox.h"

#include "SettingsWidget.generated.h"

class USlider;
class UTextBlock;
class UComboBoxString;

UCLASS(BlueprintType)
class USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 合成主播，必填
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UComboBoxString* VoiceComboBox;

	// 合成音量，可选，0-10 之间的整数
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* VolumeSlider;
	// 合成音量文本
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* VolumeText;
	
	// 合成语速，可选，0-10 之间的整数
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* SpeedSlider;
	// 合成语速文本
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SpeedText;

protected:

	// 合成主播修改
	UFUNCTION()
	void OnVoiceChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	// 合成音量修改
	UFUNCTION()
	void OnVolumeChanged(float InValue);
	
	// 合成语速修改
	UFUNCTION()
	void OnSpeedChanged(float InValue);
	
	virtual void NativeConstruct() override;
};