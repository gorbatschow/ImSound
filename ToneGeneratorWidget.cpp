#include "ToneGeneratorWidget.h"

void ToneGeneratorWidget::paint() {
  const auto generator{_generator.lock()};
  assert(generator != nullptr);

  // Paint
  SoundGeneratorWidget::paint();
  ui.frequencyModeRadio.paint();
  if (ui.frequencyModeRadio() == 0) {
    ui.frequencyIndexSpin.paint();
  } else if (ui.frequencyModeRadio() == 1) {
    ui.frequencyHertzSpin.paint();
  } else if (ui.frequencyModeRadio() == 2) {
    ui.frequencyNormalSlider.paint();
  } else {
    assert(false);
  }
  ui.frequencyHertzLabel.paint();

  // Handle
  if (ui.frequencyModeRadio.handle()) {
    if (ui.frequencyModeRadio() == 0) {
      generator->setFrequencyIndex(ui.frequencyIndexSpin());
    } else if (ui.frequencyModeRadio() == 1) {
      generator->setFrequencyHertz(ui.frequencyHertzSpin());
    } else if (ui.frequencyModeRadio() == 2) {
      generator->setFrequencyPercent(ui.frequencyNormalSlider());
    } else {
      assert(false);
    }
    ui.frequencyHertzLabel.setValue(generator->frequencyHertz());
  }

  if (ui.frequencyIndexSpin.handle()) {
    generator->setFrequencyIndex(ui.frequencyIndexSpin());
    ui.frequencyHertzLabel.setValue(generator->frequencyHertz());
  }

  if (ui.frequencyHertzSpin.handle()) {
    generator->setFrequencyHertz(ui.frequencyHertzSpin());
    ui.frequencyHertzLabel.setValue(generator->frequencyHertz());
  }

  if (ui.frequencyNormalSlider.handle()) {
    generator->setFrequencyPercent(ui.frequencyNormalSlider());
    ui.frequencyHertzLabel.setValue(generator->frequencyHertz());
  }
}

void ToneGeneratorWidget::applyStreamConfig(const RtSoundSetup &setup) {
  SoundGeneratorWidget<RtSoundToneGen>::applyStreamConfig(setup);

  auto generator{_generator.lock()};
  assert(generator);

  generator->setFrequencyPercent(ui.frequencyNormalSlider.value());
  ui.frequencyHertzLabel.setValue(generator->frequencyHertz());
}
