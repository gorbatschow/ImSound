#pragma once
#include "RtSoundSetup.h"
#include "SoundGeneratorWidget.h"
#include <RtSoundToneGen.h>
#include <memory>

class ToneGeneratorWidget : public SoundGeneratorWidget<RtSoundToneGen> {
public:
  // Constructor
  ToneGeneratorWidget(std::weak_ptr<RtSoundToneGen> generator_)
      : SoundGeneratorWidget<RtSoundToneGen>(generator_) {}

  // Destructor
  virtual ~ToneGeneratorWidget() override = default;

  // Paint
  virtual void paint() override;

protected:
  void applyStreamConfig(const RtSoundSetup &setup) override;

private:
  struct Ui {
    Imw::MultiRadioButton frequencyModeRadio{{"Index", "Hertz", "Normal"},
                                             "Frequency Mode"};
    Imw::SpinBox<int> frequencyIndexSpin{"Frequency Index"};
    Imw::SpinBox<float> frequencyHertzSpin{"Frequency Hertz"};
    Imw::ValueLabel<float> frequencyHertzLabel{"Current Frequency %.2f Hz"};
    Imw::Slider<float> frequencyNormalSlider{"Frequency Normal"};

    Ui() {
      frequencyIndexSpin.setValueLimits({1, 262144});

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
