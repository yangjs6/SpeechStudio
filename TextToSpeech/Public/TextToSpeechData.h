// CopyRight(c)2019 51VR All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "TextToSpeechData.generated.h"


// 数据基类，使用默认的Json序列化/反序列化成结构体数据
USTRUCT(BlueprintType)
struct FTTSBaseData
{
	GENERATED_BODY()
public:
	virtual ~FTTSBaseData() {}
	virtual bool ValidCheck() const { return true; }
	virtual UScriptStruct* GetStructDefinition() const { return FTTSBaseData::StaticStruct(); }
	virtual FString ToString() const;
	virtual bool FromString(const FString& InString);
};

USTRUCT(BlueprintType)
struct FTTSBaseResult : public FTTSBaseData
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetStructDefinition() const override { return FTTSBaseResult::StaticStruct(); }

	virtual bool ValidCheck() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		int status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		FString message;
};

USTRUCT(BlueprintType)
struct FTTSAuthorResultBody
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		FString token;
};

USTRUCT(BlueprintType)
struct FTTSAuthorResult : public FTTSBaseResult
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetStructDefinition() const override { return FTTSAuthorResult::StaticStruct(); }

	virtual bool ValidCheck() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		FTTSAuthorResultBody body;
};

USTRUCT(BlueprintType)
struct FTTSAuthorData : public FTTSBaseData
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetStructDefinition() const override { return FTTSAuthorData::StaticStruct(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		FString sign_text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
		int64 time;
};

USTRUCT(BlueprintType)
struct FTTSSynthData : public FTTSBaseData
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetStructDefinition() const override { return FTTSSynthData::StaticStruct(); }

	// 合成文本，必填，长度1-5000
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString text;

	// 合成主播，必填
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString vcn;

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


USTRUCT(BlueprintType)
struct FTTSSynthResultBody
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FString url;
};

USTRUCT(BlueprintType)
struct FTTSSynthResult : public FTTSBaseResult
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetStructDefinition() const override { return FTTSSynthResult::StaticStruct(); }

	virtual bool ValidCheck() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextToSpeech")
	FTTSSynthResultBody body;
};

