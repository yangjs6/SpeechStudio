#include "HttpUtils.h"

#include "Containers/Ticker.h"
#include "HTTPEndPoint.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/App.h"
#include "Misc/FeedbackContext.h"

FString  FHttpUtils::UrlEncode(const FString& InUrl)
{
	FString OutUrl = FGenericPlatformHttp::UrlEncode(InUrl);
	return OutUrl;
}

bool FHttpUtils::RequestPostString(FFeedbackContext* FeedbackContext, const FString& InContent, FString& OutResult, const FString& Host, int  Port,
	const FString& PostType, const FString& ContentType, const FString& AcceptType , IRequestPostStringCallback* Callback)
{
	FGraphEventRef GetAvailableResultEvent = FGraphEvent::CreateGraphEvent();
	FString AvailableResult;

	const FHttpEndpoint EndPoint(Host, Port);
	EndPoint.Request<EHTTPVerb::Post, FString>(PostType, InContent, ContentType, false)
		.then([&AvailableResult, GetAvailableResultEvent](FString&& Result)
			{
				AvailableResult = Result;
				TArray<FBaseGraphTask*> DummyTask;
				GetAvailableResultEvent->DispatchSubsequents(DummyTask);
			},
			[FeedbackContext, GetAvailableResultEvent](const PromiseError& InError)
			{
				FString ErrorMessage = FString::Printf(TEXT("Request Error: [%d] , %s"), InError.Which(), InError.What());
				FeedbackContext->Serialize(*ErrorMessage, ELogVerbosity::Error, FName("FHttpUtils::RequestPostString"));
				TArray<FBaseGraphTask*> DummyTask;
				GetAvailableResultEvent->DispatchSubsequents(DummyTask);
				return false;
			});

	if (Callback)
	{
		Callback->OnBegin();
	}
	while (!GetAvailableResultEvent->IsComplete())
	{
		float DeltaTime = FApp::GetDeltaTime();
		FTicker::GetCoreTicker().Tick(DeltaTime);
		if (Callback)
		{
			Callback->OnTick(DeltaTime);
		}
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FPlatformProcess::Sleep(0.5);
	}
	if (Callback)
	{
		Callback->OnEnd();
	}

	if (!AvailableResult.IsEmpty())
	{
		OutResult = AvailableResult;
		return true;
	}
	return false;
}

bool FHttpUtils::DownloadFile(FFeedbackContext* FeedbackContext, const FString& URL, const FString& SavePath)
{
	const FName FeedbackCategory("FHttpUtils::DownloadFile");
	if (URL.IsEmpty() || SavePath.IsEmpty())
	{
		FString ErrorMessage = FString::Printf(TEXT("DownloadFile URL[%s] or SavePath[%s] Error"), *URL, *SavePath);
		FeedbackContext->Serialize(*ErrorMessage, ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	FGraphEventRef GetAvailableResultEvent = FGraphEvent::CreateGraphEvent();
	TArray<uint8> AvailableResult;

	const FHttpEndpoint EndPoint(URL);
	EndPoint.Request<EHTTPVerb::Get, TArray<uint8>>("")
		.then([&AvailableResult, GetAvailableResultEvent](TArray<uint8>&& Result)
			{
				AvailableResult = Result;
				TArray<FBaseGraphTask*> DummyTask;
				GetAvailableResultEvent->DispatchSubsequents(DummyTask);
			},
			[FeedbackContext, FeedbackCategory, GetAvailableResultEvent](const PromiseError& InError)
			{
				FString ErrorMessage = FString::Printf(TEXT("DownloadFile Error: [%d] , %s"), InError.Which(), InError.What());
				FeedbackContext->Serialize(*ErrorMessage, ELogVerbosity::Error, FeedbackCategory);
				TArray<FBaseGraphTask*> DummyTask;
				GetAvailableResultEvent->DispatchSubsequents(DummyTask);
				return false;
			});

	while (!GetAvailableResultEvent->IsComplete())
	{
		//tick http
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FPlatformProcess::Sleep(0.5);
	}

	if (AvailableResult.Num() <= 0)
	{
		FeedbackContext->Serialize(*("DownloadFile Error:" + URL), ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// 检查目录是否存在，不存在则创建
	FString Path, Filename, Extension;
	FPaths::Split(SavePath, Path, Filename, Extension);
	if (!PlatformFile.DirectoryExists(*Path))
	{
		if (!PlatformFile.CreateDirectoryTree(*Path))
		{
			FeedbackContext->Serialize(*("CreateDirectoryTree Error:" + SavePath), ELogVerbosity::Error, FeedbackCategory);
			return false;
		}
	}

	// 尝试打开并写入文件，可能只读写入不成功
	IFileHandle* FileHandle = PlatformFile.OpenWrite(*SavePath);
	if (!FileHandle)
	{
		FeedbackContext->Serialize(*("OpenWrite Error:" + SavePath), ELogVerbosity::Error, FeedbackCategory);
		return false;
	}

	// 写入文件
	FileHandle->Write(AvailableResult.GetData(), AvailableResult.Num());
	FileHandle->Flush();
	delete FileHandle;
	
	return true;
}
