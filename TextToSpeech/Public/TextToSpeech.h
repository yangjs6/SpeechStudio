// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "TextToSpeechSettings.h"

#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

TEXTTOSPEECH_API DECLARE_LOG_CATEGORY_EXTERN(LogTTS, Log, All);

class FFeedbackContext;

class TEXTTOSPEECH_API FTextToSpeech
{
public:
	
	class ISynthCallback
	{
	public:
		virtual void OnBegin() = 0;
		virtual void OnTick(double DeltaTime) = 0;
		virtual void OnEnd() = 0;
	};
	
public:
	// 登录
	bool Login(FFeedbackContext* FeedbackContext, UTextToSpeechSettings* Settings = nullptr);
	
	// 判断是否登录成功
	bool IsLogin() const { return bLogin; }
	
	// 同步合成，将文本转为语音文件
	bool Synth(const FString& InText, const FString& OutSpeechFile, FFeedbackContext* FeedbackContext, ISynthCallback* Callback = nullptr, UTextToSpeechSettings* Settings = nullptr) const;
protected:
	bool bLogin = false;
	FString LoginToken;
};
