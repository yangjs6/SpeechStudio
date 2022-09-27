// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "Templates/UniquePtr.h"
#include "Templates/SharedPointer.h"
#include "UObject/Class.h"

class COMMUNICATION_API FHTTPResponse
{
public:

	FHTTPResponse(FHttpRequestPtr InRawRequest, FHttpResponsePtr InRawResponse)
	{
		this->RawRequest = InRawRequest;
		this->RawResponse = InRawResponse;
	}

	int32 GetResponseCode() const;
	EHttpRequestStatus::Type GetStatus() const;
	bool GetIsSuccessed() const;


	template<class T>
	void GetResult(T* ObjectPtr) const
	{
		check(this->RawResponse.IsValid());
		FJsonObjectConverter::JsonObjectStringToUStruct(this->RawResponse->GetContentAsString(), ObjectPtr, 0, 0);
	}

	template<class T>
	void GetResult(TArray<T>* ArrayPtr) const
	{
		check(this->RawResponse.IsValid());
		FJsonObjectConverter::JsonArrayStringToUStruct(this->RawResponse->GetContentAsString(), ArrayPtr, 0, 0);
	}

	void GetResult(TArray<uint8>* OutBuffer) const
	{
		check(this->RawResponse.IsValid());
		*OutBuffer = this->RawResponse->GetContent();
	}

	void GetResult(FString* OutString) const
	{
		*OutString = GetStringResult();
	}


	void GetResult(int32* OutInt) const
	{
		*OutInt = GetIntegerResult();
	}

	void GetResult(float* OutFloat) const
	{
		*OutFloat = GetFloatResult();
	}

	void GetResult(bool* OutFloat) const
	{
		*OutFloat = GetBooleanResult();
	}

	void GetResult(TSharedPtr<FJsonObject>* OutJsonObject) const
	{
		*OutJsonObject = GetJsonResult();
	}

private:
	FHttpRequestPtr	RawRequest;
	FHttpResponsePtr RawResponse;

	FString GetStringResult() const;
	int32 GetIntegerResult() const;
	float GetFloatResult() const;
	bool GetBooleanResult() const;
	TSharedPtr<FJsonObject> GetJsonResult() const;
};