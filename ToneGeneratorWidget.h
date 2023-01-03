#pragma once
#include "SoundGeneratorWidget.h"
#include <ImWrapper.h>
#include <RtSoundToneGen.h>
#include <memory>

class ToneGeneratorWidget : public SoundGeneratorWidget<RtSoundToneGen> {
public:
  ToneGeneratorWidget(std::weak_ptr<RtSoundToneGen> generator_)
      : SoundGeneratorWidget<RtSoundToneGen>(generator_) {
    const auto generator{generator_.lock()};
    assert(generator != nullptr);

    generator->setFrequency(ui.frequencySlider());
  }

  ~ToneGeneratorWidget() override = default;

  void paint() override final {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    SoundGeneratorWidget::paint();
    ui.frequencySlider.paint();

    if (ui.frequencySlider.handle()) {
      generator->setFrequency(ui.frequencySlider());
    }
  }

private:
  struct Ui {
    ImWrap::Slider<int> frequencySlider{"Frequency %"};

    Ui() {
      frequencySlider.setValueLimits({1, 95});
      frequencySlider.setValue(1);
    }
  };
  Ui ui;
};
