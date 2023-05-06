#include "ImSoundControlWidget.h"
#include "ImSoundControlWidgetUi.h"
#include <cmath>
#include <memory>

namespace ImSound {

ControlWidget::ControlWidget(std::shared_ptr<RtSound::IO> io)
    : _soundIO{io}
    , _ui{std::make_unique<ControlWidgetUi>()} {
  setClientName("ImSound::ControlWidget");
  _soundIO->addClient(_ui->inputDeviceCombo);
  _soundIO->addClient(_ui->outputDeviceCombo);
  _soundIO->addClient(_ui->streamStatusLine);
}

ControlWidget::~ControlWidget() = default;

void ControlWidget::loadState(const mINI::INIStructure &ini) {
  _ui->audioApiCombo.loadState(ini);
  _soundIO->startSoundEngine(_ui->audioApiCombo());
  _ui->inputDeviceCombo->loadState(ini);
  _ui->outputDeviceCombo->loadState(ini);

  _ui->inputChannelsSpin.loadState(ini);
  _ui->outputChannelsSpin.loadState(ini);
  _ui->realtimeCheck.loadState(ini);
  _ui->minLatencyCheck.loadState(ini);
  _ui->exclusiveCheck.loadState(ini);
  _ui->numBuffersSpin.loadState(ini);
  _ui->sampleRateCombo.loadState(ini);
  _ui->bufferFramesInput.loadState(ini);
  _ui->virtStreamFlag.loadState(ini);
}

void ControlWidget::saveState(mINI::INIStructure &ini) const {
  _ui->audioApiCombo.saveState(ini);
  _ui->inputDeviceCombo->saveState(ini);
  _ui->outputDeviceCombo->saveState(ini);

  _ui->inputChannelsSpin.saveState(ini);
  _ui->outputChannelsSpin.saveState(ini);
  _ui->realtimeCheck.saveState(ini);
  _ui->minLatencyCheck.saveState(ini);
  _ui->exclusiveCheck.saveState(ini);
  _ui->numBuffersSpin.saveState(ini);
  _ui->sampleRateCombo.saveState(ini);
  _ui->bufferFramesInput.saveState(ini);
  _ui->virtStreamFlag.saveState(ini);
}

void ControlWidget::paint() {
  // Paint
  _ui->audioApiCombo.paint();
  _ui->restartEngineBtn.paint();

  _ui->inputDeviceCombo->paint();
  _ui->inputChannelsSpin.paint();

  _ui->outputDeviceCombo->paint();
  _ui->outputChannelsSpin.paint();

  _ui->sampleRateCombo.paint();

  _ui->bufferFramesInput.paint();
  _ui->numBuffersSpin.paint();
  _ui->realtimeCheck.paint();
  _ui->minLatencyCheck.paint();
  _ui->exclusiveCheck.paint();
  _ui->virtStreamFlag.paint();

  ImGui::Separator();

  _ui->setupStreamBtn.paint();
  _ui->shotStreamBtn.paint();
  _ui->stopStreamBtn.paint();
  _ui->startStreamBtn.paint();
  _ui->streamStatusLine->paint();

  // Handle
  if (_ui->audioApiCombo.handle()) {
    _soundIO->startSoundEngine(_ui->audioApiCombo());
  }
  if (_ui->restartEngineBtn.handle()) {
    _soundIO->startSoundEngine(_ui->audioApiCombo());
  }
  if (_ui->setupStreamBtn.handle()) {
    _soundIO->setupSoundStream();
  }
  if (_ui->startStreamBtn.handle()) {
    _soundIO->startSoundStream();
  }
  if (_ui->stopStreamBtn.handle()) {
    _soundIO->stopSoundStream();
  }
  if (_ui->shotStreamBtn.handle()) {
    _soundIO->startSoundStream(true);
  }
}

void ControlWidget::priorityChanged(int priority) {
  _ui->inputDeviceCombo->setClientPriority(priority);
  _ui->outputDeviceCombo->setClientPriority(priority);
  _ui->streamStatusLine->setClientPriority(priority);
}

void ControlWidget::configureStream(RtSound::StreamSetup &setup) {
  setup.setInputEnabled(_ui->inputDeviceCombo->deviceEnabled());
  if ((*_ui->inputDeviceCombo).isCurrentValid()) {
    setup.setInputDeviceId((*_ui->inputDeviceCombo)().ID);
  }
  setup.setInputChannels(_ui->inputChannelsSpin(0));
  setup.setInputFirstChannel(_ui->inputChannelsSpin(1));

  setup.setOutputEnabled(_ui->outputDeviceCombo->deviceEnabled());
  if ((*_ui->inputDeviceCombo).isCurrentValid()) {
    setup.setOutputDeviceId((*_ui->outputDeviceCombo)().ID);
  }
  setup.setOutputChannels(_ui->inputChannelsSpin(0));
  setup.setOutputFirstChannel(_ui->outputChannelsSpin(1));

  setup.setSampleRate(_ui->sampleRateCombo());
  setup.setBufferFrames(_ui->bufferFramesInput());
  setup.setBuffersNum(_ui->numBuffersSpin());

  setup.clearStreamFlags();
  if (_ui->realtimeCheck()) {
    setup.addStreamFlags(RTAUDIO_SCHEDULE_REALTIME);
  }
  if (_ui->minLatencyCheck()) {
    setup.addStreamFlags(RTAUDIO_MINIMIZE_LATENCY);
  }
  if (_ui->exclusiveCheck()) {
    setup.addStreamFlags(RTAUDIO_HOG_DEVICE);
  }

  setup.setStreamVirtual(_ui->virtStreamFlag());
}

// ----------------------------------------------------------------------------

InputControlWidget::InputControlWidget(const std::shared_ptr<RtSound::IO> &io)
    : ControlWidget{io} {
  ui()->outputChannelsSpin.setVisible(false);
  ui()->outputDeviceCombo->setVisible(false);
  ui()->outputDeviceCombo->ui().deviceEnabled.setEnabled(false);
  ui()->inputDeviceCombo->ui().deviceEnabled.setValue(true);
  ui()->inputDeviceCombo->ui().deviceEnabled.setVisible(false);
}

InputControlWidget::~InputControlWidget() = default;

} // namespace ImSound
