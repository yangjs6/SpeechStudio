#include "SettingsWidget.h"

#include "TextToSpeechSettings.h"
#include "TextToSpeechWidgetSettings.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void USettingsWidget::NativeConstruct()
{
	VoiceComboBox->OnSelectionChanged.AddDynamic(this,&USettingsWidget::OnVoiceChanged);
	VolumeSlider->OnValueChanged.AddDynamic(this,&USettingsWidget::OnVolumeChanged);
	SpeedSlider->OnValueChanged.AddDynamic(this,&USettingsWidget::OnSpeedChanged);
	
	const TArray<FString>& VoiceArray = UTextToSpeechWidgetSettings::Get()->VoiceArray;
	for (int i = 0; i < VoiceArray.Num(); i++)
	{
		VoiceComboBox->AddOption(VoiceArray[i]);
	}
	if (VoiceArray.Num() > 0)
	{
		VoiceComboBox->SetSelectedIndex(0);
		OnVoiceChanged(VoiceComboBox->GetSelectedOption(), ESelectInfo::Direct);
	}
	
	VolumeSlider->SetStepSize(1);
	VolumeSlider->SetMinValue(1);
	VolumeSlider->SetMaxValue(10);
	VolumeSlider->SetValue(5);
	OnVolumeChanged(5);

	SpeedSlider->SetStepSize(1);
	SpeedSlider->SetMinValue(1);
	SpeedSlider->SetMaxValue(10);
	SpeedSlider->SetValue(5);
	OnSpeedChanged(5);

	Super::NativeConstruct();
}
void USettingsWidget::OnVoiceChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	UTextToSpeechSettings::Get()->SynthParams.vcn = SelectedItem;
}

void USettingsWidget::OnVolumeChanged(float InValue)
{
	int Volume = FMath::RoundToInt(InValue);
	VolumeText->SetText(FText::FromString(FString::FromInt(Volume)));
	UTextToSpeechSettings::Get()->SynthParams.vol = Volume;
}

void USettingsWidget::OnSpeedChanged(float InValue)
{
	int Speed = FMath::RoundToInt(InValue);
	SpeedText->SetText(FText::FromString(FString::FromInt(Speed)));
	UTextToSpeechSettings::Get()->SynthParams.speed = Speed;
}