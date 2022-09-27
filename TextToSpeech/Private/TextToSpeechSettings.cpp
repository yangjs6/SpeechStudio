#include "TextToSpeechSettings.h"

UTextToSpeechSettings* UTextToSpeechSettings::Get()
{
	return GetMutableDefault<UTextToSpeechSettings>();
}
