// Copyright Epic Games, Inc. All Rights Reserved.

#include "TextToSpeechWidgetModule.h"
#include "TextToSpeech.h"
#include "Core.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FTextToSpeechWidgetModule"

void FTextToSpeechWidgetModule::StartupModule()
{

}

void FTextToSpeechWidgetModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTextToSpeechWidgetModule, TextToSpeechWidget)
