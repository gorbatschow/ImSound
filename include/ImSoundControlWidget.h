#pragma once
#include "ImSoundSetupWidgets.h"
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <imw.h>
#include <memory>
#include <string>
#include <vector>

namespace ImSound {
class ControlWidget : public RtSound::Client
{
public:
  // Constructor
  ControlWidget(std::weak_ptr<RtSound::IO> soundIO_);

  // Destructor
  virtual ~ControlWidget() override = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  void loadWidgetState();

  // Paint
  void paint();

private:
  std::weak_ptr<RtSound::IO> _soundIO;

public:
  struct Ui {
    Ui(std::weak_ptr<RtSound::IO> soundIO)
        : streamStatusLine{soundIO} {
      inputChannelsSpin.setValueLimits({1, 100}, 0);
      inputChannelsSpin.setValueLimits({0, 100}, 1);
      outputChannelsSpin.setValueLimits({1, 100}, 0);
      outputChannelsSpin.setValueLimits({0, 100}, 1);

      restartEngineBtn.setSameLine(true);
      stopStreamBtn.setSameLine(true);
      shotStreamBtn.setSameLine(true);
      startStreamBtn.setSameLine(true);
      streamStatusLine.setSameLine(true);
      realtimeCheck.setSameLine(true);
      minLatencyCheck.setSameLine(true);
      exclusiveCheck.setSameLine(true);
      numBuffersSpin.setWidth(100.0f);
      numBuffersSpin.setValueLimits({1, 9});
    }

    Imw::Button restartEngineBtn{"Restart"};
    Imw::Button setupStreamBtn{"Setup"};
    Imw::Button startStreamBtn{"Start"};
    Imw::Button stopStreamBtn{"Stop"};
    Imw::Button shotStreamBtn{"Shot"};
    Imw::MultiSpinBox<int> inputChannelsSpin{2, "Num. Inputs / First Input"};
    Imw::MultiSpinBox<int> outputChannelsSpin{2, "Num. Outputs / First Output"};
    Imw::CheckBox realtimeCheck{"Realtime"};
    Imw::CheckBox minLatencyCheck{"Min. Latency"};
    Imw::CheckBox exclusiveCheck{"Exclusive"};
    Imw::SpinBox<int> numBuffersSpin{"Num. Buffers"};

    AudioApiCombo audioApiCombo;
    SampleRateCombo sampleRateCombo;
    BufferSizeInput bufferFramesInput;

    std::shared_ptr<SoundDeviceCombo> inputDeviceCombo{
        new SoundDeviceCombo(SoundDeviceCombo::InputDevices)};
    std::shared_ptr<SoundDeviceCombo> outputDeviceCombo{
        new SoundDeviceCombo(SoundDeviceCombo::OutputDevices)};

    StreamStatusLine streamStatusLine;
  };
  Ui ui{_soundIO};

  virtual void configureStream(RtSound::StreamSetup &) override;
};
} // namespace ImSound
