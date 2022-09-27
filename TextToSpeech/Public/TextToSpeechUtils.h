// CopyRight(c)2019 51VR All Right Reserved

#pragma once

#include "CoreMinimal.h"

class FTextToSpeechUtils
{
public:
	//获得时间 unix 时间戳，精确到毫秒,如：	1508739389393
	static int64 GetTime();
	
	//获得时间字符串，参考 GetTime
	static FString GetTimeString();
	
	// FMD5::HashBytes，bUTF8-是否将字符串转换为UTF8
	static FString HashString(const FString& String, bool bUTF8);
	
	// FBase64::Encode，bUTF8-是否将字符串转换为UTF8
	static FString EncodeString(const FString& String, bool bUTF8);

	// 拼接表字符串
	// 示例 version=1.0&appid=0b7a7a&data={"sign_text":"5a0b523e2b3dfb2a4dd84b76110f690a","time":1496628842000}
	static FString GetFormString(const TArray<FString>& Keys, const TArray<FString>& Values);
	
	// 拼接表字符串，bSortKeys 是否排序关键字
	static FString GetFormString(const TMap<FString, FString>& KeyValues, bool bSortKeys);

	// 获得签名字符串，请求参数校验 sign 值算法
	// sign=MD5.to32LowerCase(Base64.encode(AllParamsCollection.sort().toFormString(),"UTF-8")+appSecrete)；
	static FString GetSignString(const FString& InString, const FString& AppKey);
	
	// 获得签名文本，在请求授权时，需要的参数
	// sign_text = md5({appid}+{secret}+{time})，32 位小写
	static FString GetSignText(const FString& AppId, const FString& AppKey, int64 Time);
};