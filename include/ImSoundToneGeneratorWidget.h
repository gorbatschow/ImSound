#pragma once
#include "ImSoundGeneratorWidget.h"
#include "RtSoundStreamSetup.h"
#include <RtSoundToneGen.h>
#include <memory>

namespace ImSound {
class ToneGeneratorWidget : public GeneratorWidget<RtSound::ToneGen>
{
public:
  // Constructor
  ToneGeneratorWidget(std::weak_ptr<RtSound::ToneGen> generator_)
      : GeneratorWidget<RtSound::ToneGen>(generator_) {}

  // Destructor
  virtual ~ToneGeneratorWidget() override = default;

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  virtual void loadWidgetState() override {
    GeneratorWidget<RtSound::ToneGen>::loadWidgetState();

    ui.frequencyModeRadio.loadStateFromFile();
    ui.frequencyIndexSpin.loadStateFromFile();
    ui.frequencyHertzSpin.loadStateFromFile();
    ui.frequencyNormalSlider.loadStateFromFile();

    ui.frequencyModeRadio.trigger();
  }

  // Paint
  virtual void paint() override;

protected:
  void applyStreamConfig(const RtSound::StreamSetup &setup) override;

private:
  struct Ui {
    Imw::MultiRadioButton frequencyModeRadio{{"Index", "Hertz", "Normal"},
                                             "Frequency Mode"};
    Imw::SpinBox<int> frequencyIndexSpin{"Frequency Index"};
    Imw::SpinBox<float> frequencyHertzSpin{"Frequency Hertz"};
    Imw::ValueLabel<float> frequencyHertzLabel{"Current Frequency %.2f Hz"};
    Imw::Slider<float> frequencyNormalSlider{"Frequency Normal"};

    Ui() {
      frequencyIndexSpin.setValueLimits({1, 100000});

      frequencyHertzSpin.setValueLimits({1e-3f, 192e3f});
      frequencyHertzSpin.setValueStep(100.0f);
      frequencyHertzSpin.setTextFormat("%.2f");

      frequencyNormalSlider.setValueLimits({1e-3f, 1e2f});
      frequencyNormalSlider.setValue(1.0f);
      frequencyNormalSlider.setTextFormat("%.2f");
    }
  };
  Ui ui;
};
} // namespace ImSound
