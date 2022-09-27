// Copyright Epic Games, Inc. All Rights Reserved.

#include "TextToSpeechModule.h"
#include "TextToSpeech.h"
#include "Core.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FTextToSpeechModule"

void FTextToSpeechModule::StartupModule()
{

}

void FTextToSpeechModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTextToSpeechModule, TextToSpeech)
