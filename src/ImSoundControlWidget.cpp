#include "ImSoundControlWidget.h"
#include <cmath>
#include <memory>

namespace ImSound {
ControlWidget::ControlWidget(std::weak_ptr<RtSound::IO> soundIO_)
    : _soundIO(soundIO_) {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

  soundIO->streamProvider().addClient(_ui.inputDeviceCombo);
  soundIO->streamProvider().addClient(_ui.outputDeviceCombo);
}

void ControlWidget::loadWidgetState() {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

  _ui.audioApiCombo.loadStateFromFile();
  soundIO->startSoundEngine(_ui.audioApiCombo());
  _ui.inputDeviceCombo->loadStateFromFile();
  _ui.outputDeviceCombo->loadStateFromFile();

  _ui.inputChannelsSpin.loadStateFromFile();
  _ui.outputChannelsSpin.loadStateFromFile();
  _ui.realtimeCheck.loadStateFromFile();
  _ui.minLatencyCheck.loadStateFromFile();
  _ui.exclusiveCheck.loadStateFromFile();
  _ui.numBuffersSpin.loadStateFromFile();
  _ui.sampleRateCombo.loadStateFromFile();
  _ui.bufferFramesInput.loadStateFromFile();
}

void ControlWidget::paint() {
  const auto soundIO{_soundIO.lock()};
  assert(soundIO);

  // Paint
  _ui.audioApiCombo.paint();
  _ui.restartEngineBtn.paint();

  _ui.inputDeviceCombo->paint();
  _ui.inputChannelsSpin.paint();

  _ui.outputDeviceCombo->paint();
  _ui.outputChannelsSpin.paint();

  _ui.sampleRateCombo.paint();

  _ui.bufferFramesInput.paint();
  _ui.numBuffersSpin.paint();
  _ui.realtimeCheck.paint();
  _ui.minLatencyCheck.paint();
  _ui.exclusiveCheck.paint();

  ImGui::Separator();

  _ui.setupStreamBtn.paint();
  _ui.shotStreamBtn.paint();
  _ui.stopStreamBtn.paint();
  _ui.startStreamBtn.paint();
  _ui.streamStatusLine.paint();

  // Handle
  if (_ui.audioApiCombo.handle()) {
    soundIO->startSoundEngine(_ui.audioApiCombo());
  }
  if (_ui.restartEngineBtn.handle()) {
    soundIO->startSoundEngine(_ui.audioApiCombo());
  }
  if (_ui.setupStreamBtn.handle()) {
    soundIO->setupSoundStream();
  }
  if (_ui.startStreamBtn.handle()) {
    soundIO->startSoundStream();
  }
  if (_ui.stopStreamBtn.handle()) {
    soundIO->stopSoundStream();
  }
  if (_ui.shotStreamBtn.handle()) {
    soundIO->startSoundStream(true);
  }
}

void ControlWidget::configureStream(RtSound::StreamSetup &setup) {
  setup.setInputEnabled(_ui.inputDeviceCombo->deviceEnabled());
  if ((*_ui.inputDeviceCombo).isCurrentValid()) {
    setup.setInputDeviceId((*_ui.inputDeviceCombo)().ID);
  }
  setup.setInputChannels(_ui.inputChannelsSpin(0));
  setup.setInputFirstChannel(_ui.inputChannelsSpin(1));

  setup.setOutputEnabled(_ui.outputDeviceCombo->deviceEnabled());
  if ((*_ui.inputDeviceCombo).isCurrentValid()) {
    setup.setOutputDeviceId((*_ui.outputDeviceCombo)().ID);
  }
  setup.setOutputChannels(_ui.inputChannelsSpin(0));
  setup.setOutputFirstChannel(_ui.outputChannelsSpin(1));

  setup.setSampleRate(_ui.sampleRateCombo());
  setup.setBufferFrames(_ui.bufferFramesInput());
  setup.setBuffersNum(_ui.numBuffersSpin());

  setup.clearStreamFlags();
  if (_ui.realtimeCheck()) {
    setup.addStreamFlags(RTAUDIO_SCHEDULE_REALTIME);
  }
  if (_ui.minLatencyCheck()) {
    setup.addStreamFlags(RTAUDIO_MINIMIZE_LATENCY);
  }
  if (_ui.exclusiveCheck()) {
    setup.addStreamFlags(RTAUDIO_HOG_DEVICE);
  }
}
} // namespace ImSound
