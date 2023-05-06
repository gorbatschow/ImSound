#include "ImSoundSetupWidgets.h"
#include <imwrap.h>

namespace ImSound {
struct ControlWidgetUi
{
  ImWrap::Button restartEngineBtn{"Restart"};
  ImWrap::Button setupStreamBtn{"Setup"};
  ImWrap::Button startStreamBtn{"Start"};
  ImWrap::Button stopStreamBtn{"Stop"};
  ImWrap::Button shotStreamBtn{"Shot"};
  ImWrap::MultiSpinBox<int> inputChannelsSpin{2, "Num. Inputs / First Input"};
  ImWrap::MultiSpinBox<int> outputChannelsSpin{2, "Num. Outputs / First Output"};
  ImWrap::CheckBox realtimeCheck{"Realtime"};
  ImWrap::CheckBox minLatencyCheck{"Min. Latency"};
  ImWrap::CheckBox exclusiveCheck{"Exclusive"};
  ImWrap::SpinBox<int> numBuffersSpin{"Num. Buffers"};
  ImWrap::CheckBox virtStreamFlag{"Virtual Stream"};

  AudioApiCombo audioApiCombo;
  SampleRateCombo sampleRateCombo;
  BufferSizeInput bufferFramesInput;

  std::shared_ptr<SoundDeviceCombo> inputDeviceCombo{
      std::make_shared<SoundDeviceCombo>(SoundDeviceCombo::InputDevices)};
  std::shared_ptr<SoundDeviceCombo> outputDeviceCombo{
      std::make_shared<SoundDeviceCombo>(SoundDeviceCombo::OutputDevices)};
  std::shared_ptr<StreamStatusLine> streamStatusLine{
      std::make_shared<StreamStatusLine>()};

  ControlWidgetUi() {
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

  ~ControlWidgetUi() = default;
};
} // namespace ImSound
