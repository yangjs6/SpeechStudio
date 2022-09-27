#include "TextToSpeechWidgetSettings.h"

UTextToSpeechWidgetSettings::UTextToSpeechWidgetSettings()
{
	VoiceArray.Add("4c273231975916352341282bed83");
}

UTextToSpeechWidgetSettings* UTextToSpeechWidgetSettings::Get()
{
	return GetMutableDefault<UTextToSpeechWidgetSettings>();
}
