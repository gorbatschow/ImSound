#pragma once
#include "ImSoundSetupWidgets.h"
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <imw.h>
#include <memory>
#include <string>
#include <vector>

namespace ImSound {
class ControlWidget : public RtSound::Client {
public:
  // Constructor
  ControlWidget(std::shared_ptr<RtSound::IO> io) : _soundIO{io} {
    _soundIO->addClient(_ui.inputDeviceCombo);
    _soundIO->addClient(_ui.outputDeviceCombo);
    _soundIO->addClient(_ui.streamStatusLine);
  }

  // Destructor
  virtual ~ControlWidget() override = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  void loadWidgetState();

  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;

  // Paint
  void paint();

  struct Ui {
    Ui() {
      inputChannelsSpin.setValueLimits({1, 64}, 0);
      inputChannelsSpin.setValueLimits({0, 64}, 1);
      outputChannelsSpin.setValueLimits({1, 64}, 0);
      outputChannelsSpin.setValueLimits({0, 64}, 1);

      restartEngineBtn.setSameLine(true);
      stopStreamBtn.setSameLine(true);
      shotStreamBtn.setSameLine(true);
      startStreamBtn.setSameLine(true);
      streamStatusLine->setSameLine(true);
      realtimeCheck.setSameLine(true);
      minLatencyCheck.setSameLine(true);
      exclusiveCheck.setSameLine(true);
      numBuffersSpin.setWidth(100.0f);
      numBuffersSpin.setValueLimits({1, 9});

      audioApiCombo.setWidth(300.f);
      sampleRateCombo.setWidth(200.f);
      inputDeviceCombo->setWidth(300.f);
      bufferFramesInput.setWidth(200.f);
      inputChannelsSpin.setWidth(200.f);
    }

    ImWrap::Button restartEngineBtn{"Restart"};
    ImWrap::Button setupStreamBtn{"Setup"};
    ImWrap::Button startStreamBtn{"Start"};
    ImWrap::Button stopStreamBtn{"Stop"};
    ImWrap::Button shotStreamBtn{"Shot"};
    ImWrap::MultiSpinBox<int> inputChannelsSpin{2, "Num. Inputs / First Input"};
    ImWrap::MultiSpinBox<int> outputChannelsSpin{2,
                                                 "Num. Outputs / First Output"};
    ImWrap::CheckBox realtimeCheck{"Realtime"};
    ImWrap::CheckBox minLatencyCheck{"Min. Latency"};
    ImWrap::CheckBox exclusiveCheck{"Exclusive"};
    ImWrap::SpinBox<int> numBuffersSpin{"Num. Buffers"};

    AudioApiCombo audioApiCombo;
    SampleRateCombo sampleRateCombo;
    BufferSizeInput bufferFramesInput;

    std::shared_ptr<SoundDeviceCombo> inputDeviceCombo{
        std::make_shared<SoundDeviceCombo>(SoundDeviceCombo::InputDevices)};
    std::shared_ptr<SoundDeviceCombo> outputDeviceCombo{
        std::make_shared<SoundDeviceCombo>(SoundDeviceCombo::OutputDevices)};
    std::shared_ptr<StreamStatusLine> streamStatusLine{
        std::make_shared<StreamStatusLine>()};
  };

  inline Ui &ui() { return _ui; }
  inline const Ui &ui() const { return _ui; }

private:
  virtual void configureStream(RtSound::StreamSetup &) override;

private:
  std::shared_ptr<RtSound::IO> _soundIO;
  Ui _ui;
};
} // namespace ImSound
