#pragma once
#include "ImWrapper.h"
#include "SoundSetupWidgets.h"
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <memory>
#include <string>
#include <vector>

class SoundControlWidget : public RtSoundClient {
public:
  SoundControlWidget(std::weak_ptr<RtSoundIO> soundIO_);

  void paint();

private:
  std::weak_ptr<RtSoundIO> _soundIO;

public:
  struct Ui {
    Ui(std::weak_ptr<RtSoundIO> soundIO) : streamStatusLine{soundIO} {}

    ImWrap::Button restartEngineBtn{"Restart"};
    ImWrap::Button startStreamBtn{"Start"};
    ImWrap::Button stopStreamBtn{"Stop"};
    ImWrap::Button shotStreamBtn{"Shot"};
    ImWrap::SpinBoxAB<int> inputChannelsSpin{"Num. Inputs / First Input"};
    ImWrap::SpinBoxAB<int> outputChannelsSpin{"Num. Outputs / First Output"};
    ImWrap::CheckBox realtimeCheck{"Realtime"};
    ImWrap::CheckBox minLatencyCheck{"Min. Latency"};
    ImWrap::CheckBox exclusiveCheck{"Exclusive"};
    ImWrap::SpinBox<int> numBuffersSpin{"Num. Buffers"};

    AudioApiCombo audioApiCombo;
    SampleRateCombo sampleRateCombo;
    BufferSizeInput bufferFramesInput;

    std::shared_ptr<SoundDeviceCombo> inputDeviceCombo{
        new SoundDeviceCombo(SoundDeviceCombo::InputOnly)};
    std::shared_ptr<SoundDeviceCombo> outputDeviceCombo{
        new SoundDeviceCombo(SoundDeviceCombo::OutputOnly)};

    StreamStatusLine streamStatusLine;
  };
  Ui ui{_soundIO};

  virtual void configureStream(RtSoundSetup &) override;
};
