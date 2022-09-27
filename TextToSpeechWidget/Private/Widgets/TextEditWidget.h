#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/MultiLineEditableTextBox.h"

#include "TextEditWidget.generated.h"


UCLASS(BlueprintType)
class UTextEditWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 可编辑文本框
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableTextBox* TextBox;

	FString GetTextString() const;
};