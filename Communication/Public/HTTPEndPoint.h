// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Promise.hpp"
#include "HTTPCommon.h"
#include "HTTPResponse.h"
#include "HTTPRequest.h"

class COMMUNICATION_API FHttpEndpoint
{
public:

	FHttpEndpoint(const FString& Host,int32 Port = -1);

	template< EHTTPVerb Verb,typename ResultType>
	auto Request(const FString& Url, bool Authenticated = false) const
	{
		return InternalRequest<Verb>(MakeUrl(Url), Authenticated).then(GetResponseResult<ResultType>);
	}

	template< EHTTPVerb Verb, typename ResultType,typename PayLoadType>
	auto Request(const FString& Url,const PayLoadType& PayLoad, bool Authenticated = false) const
	{
		FString PayloadString;
		FJsonObjectConverter::UStructToJsonObjectString(PayLoad.StaticStruct(), &PayLoad, PayloadString, 0, 0);
		return InternalRequest<Verb>(MakeUrl(Url), PayloadString, Authenticated).then(GetResponseResult<ResultType>);
	}

	template< EHTTPVerb Verb, typename ResultType>
	auto Request(const FString& Url, const FString& PayLoad, bool Authenticated = false) const
	{
		return InternalRequest<Verb>(MakeUrl(Url), PayLoad, Authenticated).then(GetResponseResult<ResultType>);
	}

	template< EHTTPVerb Verb, typename ResultType>
	auto Request(const FString& Url, const FString& PayLoad, const FString& ContentType, bool Authenticated = false) const
	{
		return InternalRequest<Verb>(MakeUrl(Url), PayLoad, ContentType, Authenticated).then(GetResponseResult<ResultType>);
	}

	void SetAccessToken(const FString& Token) {
		this->AccessToken = Token;
	}

private:

	FString Host;

	int32 Port;

	FString AccessToken;

	FString MakeUrl(const FString& Path) const
	{
		if (Port != -1)
			return FString::Printf(TEXT("%s:%d%s"), *Host, Port, *Path);
		else
			return FString::Printf(TEXT("%s%s"), *Host, *Path);
	}

	template<EHTTPVerb Verb>
	Future<FHTTPResponse> InternalRequest(const FString& Url, bool Authenticated = false) const;

	template<EHTTPVerb Verb>
	Future<FHTTPResponse> InternalRequest(const FString& Url, const FString& Payload, bool Authenticated = false) const;

	template<EHTTPVerb Verb>
	Future<FHTTPResponse> InternalRequest(const FString& Url, const FString& Payload, const FString& ContentType, bool Authenticated = false) const;

	static Future<FHTTPResponse> ProcessRequest(TSharedPtr<class FHTTPRequestBase> NewRequest);

	template<class ResultType>
	static ResultType GetResponseResult(FHTTPResponse&& Response)
	{
		ResultType RawResult;
		Response.GetResult(&RawResult);
		return RawResult;
	}
};

template<EHTTPVerb Verb>
Future<FHTTPResponse> FHttpEndpoint::InternalRequest(const FString& Url, bool Authenticated) const
{
	auto NewRequest = MakeShared<FHTTPRequest<Verb>>(Url);

	if (Authenticated)
	{
		NewRequest->SetHeader(FString(TEXT("Authorization")), FString::Printf(TEXT("%s"), *this->AccessToken));
	}

	return ProcessRequest(NewRequest);
}

template<EHTTPVerb Verb>
Future<FHTTPResponse> FHttpEndpoint::InternalRequest(const FString& Url, const FString& Payload, bool Authenticated) const
{
	return InternalRequest(Url, Payload, FString(TEXT("application/json")), Authenticated);
}

template<EHTTPVerb Verb>
Future<FHTTPResponse> FHttpEndpoint::InternalRequest(const FString& Url, const FString& Payload, const FString& ContentType, bool Authenticated) const
{
	auto NewRequest = MakeShared<FHTTPRequest<Verb>>(Url);

	NewRequest->SetPayload(Payload);

	if (!ContentType.IsEmpty())
	{
		NewRequest->SetHeader(FString(TEXT("Content-Type")), ContentType);
	}

	if (Authenticated)
	{
		NewRequest->SetHeader(FString(TEXT("Authorization")), FString::Printf(TEXT("%s"), *this->AccessToken));
	}

	return ProcessRequest(NewRequest);
}
#include "Promise.Imp.hpp"
#include "Promise.Void.hpp"