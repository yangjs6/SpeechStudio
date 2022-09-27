#include "TextToSpeech.h"
#include "TextToSpeechData.h"
#include "TextToSpeechUtils.h"
#include "HttpUtils.h"
#include "Misc/FeedbackContext.h"

bool FTextToSpeech::Login(FFeedbackContext* FeedbackContext, UTextToSpeechSettings* Settings)
{
	const FName FeedbackCategory("FTextToSpeech::Login");	
	
	if (bLogin)
	{
		bLogin = false;
		LoginToken.Empty();
	}
	Settings = Settings ? Settings : UTextToSpeechSettings::Get();

	const FString& Host = Settings->AppParams.Host;
	const int    & Port = Settings->AppParams.Port;
	const FString& ContentType = Settings->AppParams.ContentType;
	const FString& AcceptType = Settings->AppParams.AcceptType;
	const FString& Version = Settings->AppParams.Version;
	const FString& AppId = Settings->AppParams.AppId;
	const FString& AppKey = Settings->AppParams.AppKey;
	const FString& PostType = Settings->AppParams.AuthorPostType;

	// 构造认证数据
	FTTSAuthorData AuthorData;
	// unix 时间戳，精确到毫秒,如：	1508739389393；此时间值与标准时间相差需在小于 30 分钟内
	AuthorData.time = FTextToSpeechUtils::GetTime();
	// sign_text = md5({appid}+{secret}+{time})，32 位小写
	AuthorData.sign_text = FTextToSpeechUtils::GetSignText(AppId, AppKey, AuthorData.time);

	// 构造参数列表
	TMap<FString, FString> AllParams;
	AllParams.Add(TEXT("version"), Version);
	AllParams.Add(TEXT("appid"), AppId);
	AllParams.Add(TEXT("data"), AuthorData.ToString());

	// 拼接表字符串，示例 version=1.0&appid=0b7a7a&data={"sign_text":"5a0b523e2b3dfb2a4dd84b76110f690a","time":1496628842000}
	const FString AllParamsString = FTextToSpeechUtils::GetFormString(AllParams, true);
	// sign=MD5.to32LowerCase(Base64.encode(AllParamsCollection.sort().toFormString(),"UTF-8")+appSecrete)；
	const FString SignString = FTextToSpeechUtils::GetSignString(AllParamsString, AppKey);

	// 将签名信息加入到参数列表中，并生成Post请求内容
	AllParams.Add(TEXT("sign"), SignString);
	const FString Content = FTextToSpeechUtils::GetFormString(AllParams, false);

	FeedbackContext->Serialize(*("Login Begin: " + Content),
		ELogVerbosity::Display, FeedbackCategory);
	
	// 发送Post请求，并获得结果数据
	FString Result;
	if (!FHttpUtils::RequestPostString(FeedbackContext, Content, Result, Host, Port, PostType, ContentType, AcceptType))
	{
		FeedbackContext->Serialize(*("RequestPostString Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	FTTSAuthorResult AuthorResult;	
	// 将结果数据json转换成结构体
	if (!AuthorResult.FromString(Result))
	{
		FeedbackContext->Serialize(*("ParseAuthorResult Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}
	// 验证认证数据的有效性
	if (!AuthorResult.ValidCheck())
	{
		FeedbackContext->Serialize(*("AuthorResultValidCheck Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}	
	// 保存登录token信息，以便后续使用
	LoginToken = AuthorResult.body.token;
	bLogin = true;
	
	FeedbackContext->Serialize(*("Login Successful: " + Result),
		ELogVerbosity::Display, FeedbackCategory);
	
	return true;
}

// 将FTextToSpeech::ISynthCallback包装成FHttpUtils::IRequestPostStringCallback
class FSynthCallbackProxy : public FHttpUtils::IRequestPostStringCallback
{
public:
	FSynthCallbackProxy(FTextToSpeech::ISynthCallback* InCallback)
		:SynthCallback(InCallback)
	{
		
	}	
	virtual void OnBegin() override
	{
		if (SynthCallback)
		{
			SynthCallback->OnBegin();
		}
	}
	virtual void OnTick(double DeltaTime) override
	{
		if (SynthCallback)
		{
			SynthCallback->OnTick(DeltaTime);
		}
	}
	virtual void OnEnd() override
	{
		if (SynthCallback)
		{
			SynthCallback->OnEnd();
		}
	}

protected:
	FTextToSpeech::ISynthCallback* SynthCallback = nullptr;
};

bool FTextToSpeech::Synth(const FString& InText, const FString& OutSpeechFile, FFeedbackContext* FeedbackContext, ISynthCallback* Callback, UTextToSpeechSettings* Settings) const
{
	const FName FeedbackCategory("FTextToSpeech::Synth");	
	
	if (!bLogin || LoginToken.IsEmpty())
	{
		FeedbackContext->Serialize(TEXT("Not Login YET!"),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}
	Settings = Settings ? Settings : UTextToSpeechSettings::Get();

	const FString& Host = Settings->AppParams.Host;
	const int    & Port = Settings->AppParams.Port;
	const FString& ContentType = Settings->AppParams.ContentType;
	const FString& AcceptType = Settings->AppParams.AcceptType;
	const FString& Version = Settings->AppParams.Version;
	const FString& AppId = Settings->AppParams.AppId;
	const FString& AppKey = Settings->AppParams.AppKey;
	const FString& PostType = Settings->AppParams.SynthPostType;
	const FString TimeString = FTextToSpeechUtils::GetTimeString();

	const FString ServiceString = "tts/synth";
	const FString TokenString = LoginToken;
	const FString EncodeToken = FHttpUtils::UrlEncode(TokenString);

	// 构造合成参数
	FTTSSynthData SynthData;
	SynthData.text = InText;
	SynthData.vcn = Settings->SynthParams.vcn;
	SynthData.vol = Settings->SynthParams.vol;
	SynthData.speed = Settings->SynthParams.speed;
	SynthData.auf = Settings->SynthParams.auf;
	SynthData.aue = Settings->SynthParams.aue;
	SynthData.ttstype = Settings->SynthParams.ttstype;
	SynthData.enginetype = Settings->SynthParams.enginetype;
	FString DataString = SynthData.ToString();

	// 构造参数列表
	TMap<FString, FString> AllParams;
	AllParams.Add(TEXT("service"), ServiceString);
	AllParams.Add(TEXT("appid"), AppId);
	AllParams.Add(TEXT("version"), Version);
	AllParams.Add(TEXT("data"), DataString);
	AllParams.Add(TEXT("time"), TimeString);
	AllParams.Add(TEXT("token"), TokenString);

	// 拼接表字符串，示例 version=1.0&appid=0b7a7a&data={"sign_text":"5a0b523e2b3dfb2a4dd84b76110f690a","time":1496628842000}
	const FString AllParamsString = FTextToSpeechUtils::GetFormString(AllParams, true);
	// sign=MD5.to32LowerCase(Base64.encode(AllParamsCollection.sort().toFormString(),"UTF-8")+appSecrete)；
	const FString SignString = FTextToSpeechUtils::GetSignString(AllParamsString, AppKey);
	
	// 将签名信息加入到参数列表中，并生成Post请求内容
	AllParams.Empty();
	AllParams.Add(TEXT("version"), Version);
	AllParams.Add(TEXT("appid"), AppId);
	AllParams.Add(TEXT("time"), TimeString);
	AllParams.Add(TEXT("token"), EncodeToken);
	AllParams.Add(TEXT("data"), DataString);	
	AllParams.Add(TEXT("sign"), SignString);	
	
	FString Content = FTextToSpeechUtils::GetFormString(AllParams, false);

	FeedbackContext->Serialize(*("Synth Begin: " + Content),
		ELogVerbosity::Display, FeedbackCategory);
	
	// 发送Post请求，并获得结果数据
	FSynthCallbackProxy CallbackProxy(Callback);
	FString Result;
	if (!FHttpUtils::RequestPostString(FeedbackContext, Content, Result, Host, Port, PostType, ContentType, AcceptType, &CallbackProxy))
	{
		FeedbackContext->Serialize(*("RequestPostString Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	FTTSSynthResult SynthResult;	
	// 将结果数据json转换成结构体
	if (!SynthResult.FromString(Result))
	{
		FeedbackContext->Serialize(*("ParseSynthResult Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}
	// 验证合成数据的有效性
	if (!SynthResult.ValidCheck())
	{
		FeedbackContext->Serialize(*("ResultValidCheck Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	if (!FHttpUtils::DownloadFile(FeedbackContext, SynthResult.body.url, OutSpeechFile))
	{
		FeedbackContext->Serialize(*("DownloadFile Error : " + Result),
			ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	FeedbackContext->Serialize(*("Synth Successful: " + Result),
		ELogVerbosity::Display, FeedbackCategory);
	
	return true;
}

DEFINE_LOG_CATEGORY(LogTTS);
