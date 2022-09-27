#include "HTTPCommon.h"

FString HttpVerbStrings[(int32)EHTTPVerb::HTTP_VERB_MAX] =
{
	FString("GET"),
	FString("POST"),
	FString("PATCH"),
	FString("PUT"),
	FString("DELETE"),
};