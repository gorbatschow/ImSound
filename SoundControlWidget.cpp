#include "SoundControlWidget.h"
#include <cmath>
#include <imgui.h>
#include <memory>

SoundControlWidget::SoundControlWidget(std::weak_ptr<RtSoundIO> soundIO_)
    : _soundIO(soundIO_) {
  const auto soundIO{_soundIO.lock().get()};
  assert(soundIO != nullptr);
  soundIO->streamProvider().addClient(ui.inputDeviceCombo);
  soundIO->streamProvider().addClient(ui.outputDeviceCombo);
}

void SoundControlWidget::paint() {
  const auto soundIO{_soundIO.lock().get()};
  assert(soundIO != nullptr);

  // Paint
  ui.audioApiCombo.paint();
  ui.restartEngineBtn.paint();
  ui.inputDeviceCombo->paint();
  ui.inputChannelsSpin.paint();
  ui.outputDeviceCombo->paint();
  ui.outputChannelsSpin.paint();
  ui.sampleRateCombo.paint();
  ui.bufferFramesInput.paint();
  ui.numBuffersSpin.paint();
  ui.realtimeCheck.paint();
  ui.minLatencyCheck.paint();
  ui.exclusiveCheck.paint();
  ImGui::Separator();
  ui.setupStreamBtn.paint();
  ui.shotStreamBtn.paint();
  ui.stopStreamBtn.paint();
  ui.startStreamBtn.paint();
  ui.streamStatusLine.paint();

  // Handle
  if (ui.restartEngineBtn.handle()) {
    soundIO->startSoundEngine(ui.audioApiCombo());
  }
  if (ui.setupStreamBtn.handle()) {
    soundIO->setupSoundStream();
  }
  if (ui.startStreamBtn.handle()) {
    soundIO->startSoundStream();
  }
  if (ui.stopStreamBtn.handle()) {
    soundIO->stopSoundStream();
  }
  if (ui.shotStreamBtn.handle()) {
    soundIO->startSoundStream(true);
  }
}

void SoundControlWidget::configureStream(RtSoundSetup &setup) {
  setup.setInputEnabled(ui.inputDeviceCombo->deviceEnabled());
  setup.setInputDeviceId((*ui.inputDeviceCombo)().ID);
  setup.setInputChannels(ui.inputChannelsSpin(0));
  setup.setInputFirstChannel(ui.inputChannelsSpin(1));

  setup.setOutputEnabled(ui.outputDeviceCombo->deviceEnabled());
  setup.setOutputDeviceId((*ui.outputDeviceCombo)().ID);
  setup.setOutputChannels(ui.inputChannelsSpin(0));
  setup.setOutputFirstChannel(ui.outputChannelsSpin(1));

  setup.setSampleRate(ui.sampleRateCombo());
  setup.setBufferFrames(ui.bufferFramesInput());
  setup.setBuffersNum(ui.numBuffersSpin());

  setup.clearStreamFlags();
  if (ui.realtimeCheck()) {
    setup.addStreamFlags(RTAUDIO_SCHEDULE_REALTIME);
  }
  if (ui.minLatencyCheck()) {
    setup.addStreamFlags(RTAUDIO_MINIMIZE_LATENCY);
  }
  if (ui.exclusiveCheck()) {
    setup.addStreamFlags(RTAUDIO_HOG_DEVICE);
  }
}
