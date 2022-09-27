#include "TextToSpeechUtils.h"

#include <string>
#include "Misc/App.h"
#include "Misc/Base64.h"
#include "Misc/SecureHash.h"

int64 FTextToSpeechUtils::GetTime()
{
	const FDateTime DateTimeNow = FDateTime::Now();
	const int64 UnixTimestamp = (DateTimeNow.GetTicks() - FDateTime(1970, 1, 1, 8).GetTicks()) / ETimespan::TicksPerMillisecond;
	return UnixTimestamp;
}

FString FTextToSpeechUtils::GetTimeString()
{
	const int64 Time = GetTime();
	FString TimeString = FString::Printf(TEXT("%lld"), Time);
	return TimeString;
}

FString FTextToSpeechUtils::HashString(const FString& String, bool bUTF8)
{
	const std::string stdString = bUTF8 ? TCHAR_TO_UTF8(*String) : TCHAR_TO_ANSI(*String);
	return FMD5::HashBytes((const uint8*)stdString.c_str(), stdString.length());
}

FString FTextToSpeechUtils::EncodeString(const FString& String, bool bUTF8)
{
	const std::string stdString = bUTF8 ? TCHAR_TO_UTF8(*String) : TCHAR_TO_ANSI(*String);
	return FBase64::Encode((const uint8*)stdString.c_str(), stdString.length());
}

FString FTextToSpeechUtils::GetFormString(const TMap<FString, FString>& KeyValues, bool bSortKeys)
{
	TArray<FString> Keys;
	KeyValues.GenerateKeyArray(Keys);
	if (bSortKeys)
	{
		Keys.Sort();
	}
	TArray<FString> Values;
	Values.SetNum(Keys.Num());
	for(int i = 0; i < Keys.Num(); i++)
	{
		Values[i] = KeyValues[Keys[i]];
	}

	return GetFormString(Keys, Values);
}

FString FTextToSpeechUtils::GetFormString(const TArray<FString>& Keys, const TArray<FString>& Values)
{
	FString ParseString;
	for (int i = 0; i < Keys.Num(); i++)
	{
		FString key = Keys[i];
		FString value = Values[i];
		// 拼接时，不包括最后一个&字符
		if (i == Keys.Num() - 1)
		{
			ParseString.Append(key);
			ParseString.Append("=");
			ParseString.Append(value);
		}
		else
		{
			ParseString.Append(key);
			ParseString.Append("=");
			ParseString.Append(value);
			ParseString.Append("&");
		}
	}
	return ParseString;
}

FString FTextToSpeechUtils::GetSignString(const FString& InString, const FString& AppKey)
{
	const FString EncodeStr = EncodeString(InString, true);
	FString SignString = HashString(EncodeStr + AppKey, true);
	return SignString;
}

FString FTextToSpeechUtils::GetSignText(const FString& AppId, const FString& AppKey, int64 Time)
{
	const FString TimeString = FString::Printf(TEXT("%s%s%lld"), *AppId, *AppKey, Time);
	FString SignText = HashString(TimeString, true);
	return SignText;
}

