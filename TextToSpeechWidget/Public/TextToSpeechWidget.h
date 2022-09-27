#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Windows/WindowsFeedbackContext.h"

#include "TextToSpeechWidget.generated.h"

class UButton;
class UCircularThrobber;
class UAudioPlayerWidget;
class UTextEditWidget;
class USettingsWidget;

class FTextToSpeech;
class UTextToSpeechSettings;

UCLASS(BlueprintType)
class UTextToSpeechWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	// 登录按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LoginButton;

	// 合成按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* SynthButton;

	// 等待
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* WattingThrobber;
	
	// 音频播放器
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAudioPlayerWidget* AudioPlayerWidget;

	// 文本编辑器
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextEditWidget* TextEditWidget;

	// 配置编辑器
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USettingsWidget* SettingsWidget;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 登录
	UFUNCTION()
	void OnLogin();
	// 将文本转换成语音
	UFUNCTION()
	void OnSynth();

	// 刷新窗口状态
	void RefeshWidget();
	
	UPROPERTY(Transient)
	UTextToSpeechSettings* TextToSpeechSettings = nullptr;	
	TSharedPtr<FTextToSpeech> TextToSpeechPtr;

	FFeedbackContext* FeedbackContext;
};
