#include "SoundControlWidget.h"
#include <cmath>
#include <imgui.h>
#include <memory>

namespace ImSound {
SoundControlWidget::SoundControlWidget(std::weak_ptr<RtSound::IO> soundIO_)
    : _soundIO(soundIO_) {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

  soundIO->streamProvider().addClient(ui.inputDeviceCombo);
  soundIO->streamProvider().addClient(ui.outputDeviceCombo);
}

void SoundControlWidget::loadWidgetState() {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

  ui.audioApiCombo.loadStateFromFile();
  soundIO->startSoundEngine(ui.audioApiCombo());
  ui.inputDeviceCombo->loadStateFromFile();
  ui.outputDeviceCombo->loadStateFromFile();

  ui.inputChannelsSpin.loadStateFromFile();
  ui.outputChannelsSpin.loadStateFromFile();
  ui.realtimeCheck.loadStateFromFile();
  ui.minLatencyCheck.loadStateFromFile();
  ui.exclusiveCheck.loadStateFromFile();
  ui.numBuffersSpin.loadStateFromFile();
  ui.sampleRateCombo.loadStateFromFile();
  ui.bufferFramesInput.loadStateFromFile();
}

void SoundControlWidget::paint() {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

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
  if (ui.audioApiCombo.handle()) {
    soundIO->startSoundEngine(ui.audioApiCombo());
  }
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

void SoundControlWidget::configureStream(RtSound::StreamSetup &setup) {
  setup.setInputEnabled(ui.inputDeviceCombo->deviceEnabled());
  if ((*ui.inputDeviceCombo).isCurrentValid()) {
    setup.setInputDeviceId((*ui.inputDeviceCombo)().ID);
  }
  setup.setInputChannels(ui.inputChannelsSpin(0));
  setup.setInputFirstChannel(ui.inputChannelsSpin(1));

  setup.setOutputEnabled(ui.outputDeviceCombo->deviceEnabled());
  if ((*ui.inputDeviceCombo).isCurrentValid()) {
    setup.setOutputDeviceId((*ui.outputDeviceCombo)().ID);
  }
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
} // namespace ImSound
