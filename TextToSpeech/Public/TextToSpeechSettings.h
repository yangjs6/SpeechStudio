// CopyRight(c)2019 51VR All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TextToSpeechSettings.generated.h"

USTRUCT(BlueprintType)
struct FTTSAppParams
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString Host = "https://openapi.xfyousheng.com";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int Port = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString ContentType = "application/x-www-form-urlencoded";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString AcceptType = "application/json";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString Version = "1.0";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString AppId = "VJS1R5L9";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString AppKey = "b0af890b7d0fee365a46dad3ebbc46ec";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString AuthorPostType = "/author";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString SynthPostType = "/tts/synth";
};


USTRUCT(BlueprintType)
struct FTTSSynthParams
{
	GENERATED_USTRUCT_BODY()

	// 合成主播，必填
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString vcn = "4c273231975916352341282bed83";
	
	// 合成音量，可选，0-10 之间的整数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int vol = 5;
	
	// 合成语速，可选，0-10 之间的整数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int speed = 5;
	
	// 音频采样率，可选，0-8000，1-11050，2-16000
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int auf = 2;
	
	// 是否压缩，可选，0-不压缩，返回pcm数据，1-压缩，返回mpeg数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int aue = 1;
	
	// 默认0 若调用x2主播该值传2, 3的话 使用传入的vcn来合成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	int ttstype = 1;
	
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString enginetype = "ptts";
};


UCLASS(BlueprintType, config = TextToSpeech, DefaultConfig)
class TEXTTOSPEECH_API UTextToSpeechSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	static UTextToSpeechSettings* Get();

	virtual FName GetCategoryName() const override { return FName("SpeechStudio"); }

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "TextToSpeech")
		FTTSAppParams AppParams;

	UPROPERTY(GlobalConfig, EditAnywhere, Category = "TextToSpeech")
		FTTSSynthParams SynthParams;
};



