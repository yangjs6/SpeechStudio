#include "HTTPEndPoint.h"
#include "HTTPRequest.h"
#include "HttpModule.h"
#include "Templates/SharedPointer.h"
#include "Promise.Imp.hpp"
#include "Promise.Void.hpp"
#include "Async/TaskGraphInterfaces.h"

class FDelayTask
{
public:
	FORCEINLINE FDelayTask(float InDelayTime)
		: DelayTime(InDelayTime)
	{
	}
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDelayTask, STATGROUP_TaskGraphTasks);
	}
	static FORCEINLINE ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::AnyHiPriThreadHiPriTask;
	}
	static FORCEINLINE ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }
	void FORCENOINLINE DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		FPlatformProcess::Sleep(DelayTime);
	}
private:
	float DelayTime;
};

class FTickRequestTask
{
public:
	FORCEINLINE FTickRequestTask(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> InRequest)
		: StartThread(ENamedThreads::GameThread/*FTaskGraphInterface::Get().GetCurrentThreadIfKnown()*/)
		, Request(InRequest)
	{
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTickRequestTask, STATGROUP_TaskGraphTasks);
	}

	FORCEINLINE ENamedThreads::Type GetDesiredThread()
	{
		return StartThread;
	}

	static FORCEINLINE ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void FORCENOINLINE DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		const float TickInternal = 0.001f;
		Request->Tick(TickInternal);
		if(Request->GetStatus() == EHttpRequestStatus::Processing)
		{
			FGraphEventArray Prerequisites;
			FGraphEventArray PrerequisitesNext;
			Prerequisites.Add(MyCompletionGraphEvent);
			PrerequisitesNext.Add(TGraphTask<FDelayTask>::CreateTask(&Prerequisites, ENamedThreads::AnyThread).ConstructAndDispatchWhenReady(TickInternal));
			TGraphTask<FTickRequestTask>::CreateTask(&PrerequisitesNext,
				FTaskGraphInterface::Get().GetCurrentThreadIfKnown()).ConstructAndDispatchWhenReady(Request);
		}
	}

private:
	ENamedThreads::Type StartThread;
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request;
};

Future<FHTTPResponse> FHttpEndpoint::ProcessRequest(TSharedPtr<FHTTPRequestBase> NewRequest)
{
	auto Request = NewRequest->GetRequest();
	Request->OnProcessRequestComplete().BindLambda([NewRequest](FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool bSuccessed) {
		TUniquePtr<FHTTPResponse> Response = MakeUnique<FHTTPResponse>(RequestPtr, ResponsePtr);
		if (bSuccessed && Response->GetIsSuccessed() && Response->GetResponseCode() == HTTP_STATUS_OK)
		{
			NewRequest->CommunicationPromise->resolve(*Response);
		}
		else if (Response->GetIsSuccessed())
		{
			FString ErrorMessage;
			Response->GetResult(&ErrorMessage);
			NewRequest->CommunicationPromise->reject(PromiseError(*ErrorMessage, Response->GetResponseCode()));
		}
		else
		{
			auto ErrorCode = Response->GetResponseCode();
			NewRequest->CommunicationPromise->reject(PromiseError(*FString::Printf(TEXT("Http Error : %d"), ErrorCode), ErrorCode));
		}
	});
	Request->ProcessRequest();

	TGraphTask<FTickRequestTask>::CreateTask(nullptr, FTaskGraphInterface::Get()
		.GetCurrentThreadIfKnown()).ConstructAndDispatchWhenReady(Request);

	return NewRequest->Future();
}

FHttpEndpoint::FHttpEndpoint(const FString& InHost, int32 InPort)
	: Host(InHost)
	, Port(InPort)
{

}

