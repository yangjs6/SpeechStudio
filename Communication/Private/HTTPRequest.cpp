#include "HTTPRequest.h"
#include "HttpModule.h"
#include "Templates/SharedPointer.h"
#include "Promise.Imp.hpp"
#include "Promise.Void.hpp"

FHTTPRequestBase::FHTTPRequestBase()
	: Request(FHttpModule::Get().CreateRequest())
	, CommunicationPromise(MakeUnique<Promise<FHTTPResponse>>())
{

}

TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> FHTTPRequestBase::GetRequest() const
{
	return Request;
}

Future<FHTTPResponse> FHTTPRequestBase::Future()
{
	Request.Reset();
	return CommunicationPromise->future();
}