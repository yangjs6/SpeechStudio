#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/EditableTextBox.h"

#include "AudioPlayerWidget.generated.h"

class UButton;
class UTextBlock;
class USlider;
class FFeedbackContext;

// 音频播放器窗口
UCLASS(BlueprintType)
class UAudioPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	// 加载按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* LoadButton;

	// 文件路径文本编辑框
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* FileTextBox;

	// 播放按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PlayButton;

	// 暂停按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* PauseButton;
	
	// 停止按钮
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* StopButton;

	// 时间条滑块
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USlider* TimeSlider;

	// 当前播放时刻文本
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* PlaybackTimeText;

	// 总时长文本
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DurationText;

public:
	// 加载Mp3声音文件
	bool LoadMp3Sound(const FString& InMp3File, FFeedbackContext* FeedbackContext);
	
	// 更新Sound
	UFUNCTION(BlueprintCallable, Category = "AudioPlayer")
	void UpdateSound(USoundBase* InSound);

	// 设置播放时刻
	void SetPlaybackTime(float InTime) const;	
	// 获得播放时刻
	float GetPlaybackTime() const;
	// 设置播放时长
	void SetDuration(float InTime) const;	
	// 获得播放时长
	float GetDuration() const;
	
protected:	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 加载
	UFUNCTION()
	void OnLoad();
	// 播放
	UFUNCTION()
	void OnPlay();
	// 暂停
	UFUNCTION()
	void OnPause();
	// 停止
	UFUNCTION()
	void OnStop();
	// 时间跳转
	UFUNCTION()
	void OnTimeChanged(float InValue);

	// 播放状态改变
	UFUNCTION()
	void OnPlayStateChaged(EAudioComponentPlayState PlayState);
	
	// 刷新窗口状态
	void RefeshWidget();
	
	UPROPERTY(Transient)
	UAudioComponent* AudioComponent = nullptr;
};
