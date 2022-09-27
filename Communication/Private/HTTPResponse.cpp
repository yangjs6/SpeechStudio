#include "HTTPResponse.h"

int32 FHTTPResponse::GetResponseCode() const
{
	if (!this->RawResponse.IsValid())
		return -1;

	return this->RawResponse->GetResponseCode();
}

EHttpRequestStatus::Type FHTTPResponse::GetStatus() const
{
	return this->RawRequest->GetStatus();
}

bool FHTTPResponse::GetIsSuccessed() const
{
	return this->GetStatus() == EHttpRequestStatus::Succeeded
		&& this->RawResponse.IsValid();
}

FString FHTTPResponse::GetStringResult() const
{
	check(this->RawResponse.IsValid());
	return this->RawResponse->GetContentAsString();
}

TSharedPtr<FJsonObject> FHTTPResponse::GetJsonResult() const
{
	check(this->RawResponse.IsValid());

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(this->RawResponse->GetContentAsString());

	FJsonSerializer::Deserialize(Reader, JsonObject);

	return JsonObject;
}

int32 FHTTPResponse::GetIntegerResult() const
{
	check(this->RawResponse.IsValid());
	return FCString::Atoi(*this->RawResponse->GetContentAsString());
}

float FHTTPResponse::GetFloatResult() const
{
	check(this->RawResponse.IsValid());
	return FCString::Atof(*this->RawResponse->GetContentAsString());
}

bool FHTTPResponse::GetBooleanResult() const
{
	check(this->RawResponse.IsValid());
	return this->RawResponse->GetContentAsString().Equals(FString("true"));
}

