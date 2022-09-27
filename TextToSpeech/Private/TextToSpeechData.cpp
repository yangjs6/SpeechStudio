#include "TextToSpeechData.h"
#include "JsonObjectConverter.h"

FString FTTSBaseData::ToString() const
{
	FString OutString;
	UScriptStruct* StructDefinition = GetStructDefinition();
	if (!FJsonObjectConverter::UStructToJsonObjectString(StructDefinition, this, OutString, 0, 0))
	{
		return FString();
	}

	OutString.RemoveSpacesInline();
	OutString.ReplaceInline(TEXT("\n"),TEXT(""));
	OutString.ReplaceInline(TEXT("\t"),TEXT(""));
	OutString.ReplaceInline(TEXT("\r"),TEXT(""));
	return OutString;
}

bool FTTSBaseData::FromString(const FString& InString)
{
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(InString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
	{
		return false;
	}

	UScriptStruct* StructDefinition = GetStructDefinition();
	if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructDefinition, this, 0, 0))
	{
		return false;
	}
	return true;
}

bool FTTSBaseResult::ValidCheck() const
{
	if (status != 200)
	{
		return false;
	}
	return true;
}

bool FTTSAuthorResult::ValidCheck() const
{
	if (!FTTSBaseResult::ValidCheck())
	{
		return false;
	}
	if (body.token.Len() <= 0)
	{
		return false;
	}
	return true;
}

bool FTTSSynthResult::ValidCheck() const
{
	if (!FTTSBaseResult::ValidCheck())
	{
		return false;
	}
	if (body.url.Len() <= 0)
	{
		return false;
	}
	return true;
}
