#pragma once
#include "SoundSetupWidgets.h"
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <imw.h>
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

    Imw::Button restartEngineBtn{"Restart"};
    Imw::Button startStreamBtn{"Start"};
    Imw::Button stopStreamBtn{"Stop"};
    Imw::Button shotStreamBtn{"Shot"};
    Imw::SpinBoxAB<int> inputChannelsSpin{"Num. Inputs / First Input"};
    Imw::SpinBoxAB<int> outputChannelsSpin{"Num. Outputs / First Output"};
    Imw::CheckBox realtimeCheck{"Realtime"};
    Imw::CheckBox minLatencyCheck{"Min. Latency"};
    Imw::CheckBox exclusiveCheck{"Exclusive"};
    Imw::SpinBox<int> numBuffersSpin{"Num. Buffers"};

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
