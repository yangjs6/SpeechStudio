// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MP3ImporterLibrary.generated.h"

class USoundWave;

UCLASS()
class MP3IMPORTER_API UMP3ImporterLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 创建mp3的声音
	UFUNCTION(BlueprintCallable, Category = "MP3Importer")
	static USoundWave* CreateMp3Sound(const FString& Filename, FString& ErrorMessage);
};


