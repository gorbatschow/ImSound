#include "ImSoundToneGeneratorWidget.h"

namespace ImSound {
void ToneGeneratorWidget::paint() {
  const auto generator{_generator.lock()};
  assert(generator != nullptr);

  // Paint
  GeneratorWidget::paint();
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
      ui.frequencyIndexSpin.trigger();
    } else if (ui.frequencyModeRadio() == 1) {
      ui.frequencyHertzSpin.trigger();
    } else if (ui.frequencyModeRadio() == 2) {
      ui.frequencyNormalSlider.trigger();
    } else {
      assert(false);
    }
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

void ToneGeneratorWidget::applyStreamConfig(const RtSound::StreamSetup &setup) {
  GeneratorWidget<RtSound::ToneGen>::applyStreamConfig(setup);

  auto generator{_generator.lock()};
  assert(generator);

  ui.frequencyModeRadio.trigger();
}
} // namespace ImSound
