// CopyRight(c)2019 51VR All Right Reserved

#pragma once

#include "CoreMinimal.h"

class FFeedbackContext;

class FHttpUtils
{
public:
	class IRequestPostStringCallback
	{
	public:
		virtual void OnBegin() = 0;
		virtual void OnTick(double DeltaTime) = 0;
		virtual void OnEnd() = 0;
	};
	
	// Url编码
	static FString UrlEncode(const FString& InUrl);
	// 请求Post，返回字符串结果
	static bool RequestPostString(FFeedbackContext* FeedbackContext, const FString& InContent, FString& OutResult, const FString& Host, int  Port = -1,
		const FString& PostType = FString(), const FString& ContentType = FString(), const FString& AcceptType = FString(), IRequestPostStringCallback* Callback = nullptr);
	// 下载文件
	static bool DownloadFile(FFeedbackContext* FeedbackContext, const FString& URL, const FString& SavePath);
};