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
    ui.frequencySlider.paint();
    ui.fequencyLabel.paint();

    if (ui.frequencySlider.handle()) {
      generator->setFrequencyPercent(ui.frequencySlider());
      ui.fequencyLabel.setValue(generator->frequencyHertz());
    }
  }

protected:
  void applyStreamConfig(const RtSoundSetup &setup) override {
    SoundGeneratorWidget<RtSoundToneGen>::applyStreamConfig(setup);

    auto generator{_generator.lock()};
    assert(generator);

    generator->setFrequencyPercent(ui.frequencySlider.value());
    ui.fequencyLabel.setValue(generator->frequencyHertz());
  }

private:
  float _Fs{};

  struct Ui {
    Imw::Slider<int> frequencySlider{"Frequency %"};
    Imw::ValueLabel<float> fequencyLabel{"%.2f Hz"};

    Ui() {
      frequencySlider.setValueLimits({1.0f, 100.0f});
      frequencySlider.setValue(1.0f);
      fequencyLabel.setSameLine(true);
    }
  };
  Ui ui;
};
