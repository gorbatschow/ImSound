#pragma once
#include "SoundGeneratorWidget.h"
#include <RtSoundToneGen.h>
#include <memory>

class ToneGeneratorWidget : public SoundGeneratorWidget<RtSoundToneGen> {
public:
  ToneGeneratorWidget(std::weak_ptr<RtSoundToneGen> generator_)
      : SoundGeneratorWidget<RtSoundToneGen>(generator_) {}

  ~ToneGeneratorWidget() override = default;

  void paint() override final {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    SoundGeneratorWidget::paint();
    ui.frequencyModeRadio.paint();
    if (ui.frequencyModeRadio() == 0) {
      ui.frequencyPercentSlider.paint();
    } else if (ui.frequencyModeRadio() == 1) {
      ui.frequencyIndexSpin.paint();
    } else {
      ui.frequencyExactSpin.paint();
    }

    // ui.frequencyLabel.paint();

    if (ui.frequencyPercentSlider.handle()) {
      generator->setFrequencyPercent(ui.frequencyPercentSlider());
      ui.frequencyLabel.setValue(generator->frequencyHertz());
    }
  }

protected:
  void applyStreamConfig(const RtSoundSetup &setup) override {
    SoundGeneratorWidget<RtSoundToneGen>::applyStreamConfig(setup);

    auto generator{_generator.lock()};
    assert(generator);

    generator->setFrequencyPercent(ui.frequencyPercentSlider.value());
    ui.frequencyLabel.setValue(generator->frequencyHertz());
  }

private:
  struct Ui {
    Imw::Slider<float> frequencyPercentSlider{"Frequency %"};
    Imw::SpinBox<int> frequencyIndexSpin{"Frequency Idx"};
    Imw::SpinBox<float> frequencyExactSpin{"Frequency Hz"};
    Imw::ValueLabel<float> frequencyLabel{"%.2f Hz"};
    Imw::MultiRadioButton frequencyModeRadio{{"Percent", "Index", "Exact"},
                                             "Frequency Mode"};

    Ui() {
      frequencyPercentSlider.setValueLimits({1e-3f, 1e2f});
      frequencyPercentSlider.setValue(1.0f);
      frequencyPercentSlider.setTextFormat("%.2f");
      frequencyExactSpin.setTextFormat("%.2f");
      // frequencyLabel.setSameLine(true);
    }
  };
  Ui ui;
};
