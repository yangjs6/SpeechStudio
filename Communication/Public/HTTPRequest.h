#pragma once

#include "Promise.hpp"
#include "HTTPCommon.h"
#include "HTTPResponse.h"

class COMMUNICATION_API FHTTPRequestBase
{
	friend class FHttpEndpoint;
public:
	FHTTPRequestBase();

	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> GetRequest() const;

	Future<FHTTPResponse> Future();
protected:
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> Request;
	TUniquePtr<Promise<FHTTPResponse>> CommunicationPromise;
};

template<EHTTPVerb Verb>
class FHTTPRequest : public FHTTPRequestBase
{
public:
	FHTTPRequest(const FString& Url)
	{
		Request->SetVerb(HttpVerbStrings[Verb]);
		Request->SetURL(Url);
	}

	void SetPayload(const FString& Payload)
	{
		Request->SetContentAsString(Payload);
	}

	void SetHeader(const FString& HeaderName, const FString& HeaderValue) 
	{
		Request->SetHeader(HeaderName, HeaderValue);
	}	
};