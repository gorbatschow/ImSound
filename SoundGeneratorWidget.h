#pragma once
#include <RtSoundNoiseGen.h>
#include <imw.h>

template <class T> class SoundGeneratorWidget {
public:
  SoundGeneratorWidget(std::weak_ptr<T> generator_) : _generator(generator_) {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    _generator.lock()->setAmplitude(ui.amplitudeSlider());
  }

  virtual ~SoundGeneratorWidget() {}

  virtual void paint() {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    // Paint
    ui.inputChannel.paint();
    ImGui::SameLine();
    ui.inputEnabled.paint();
    ui.outputChannel.paint();
    ImGui::SameLine();
    ui.outputEnabled.paint();
    ui.amplitudeSlider.paint();

    // Handle
    if (ui.inputEnabled.handle()) {
      generator->setInputEnabled(ui.inputEnabled());
    }

    if (ui.inputChannel.handle()) {
      generator->setInputChannel(ui.inputChannel());
    }

    if (ui.outputEnabled.handle()) {
      generator->setOutputEnabled(ui.outputEnabled());
    }

    if (ui.outputChannel.handle()) {
      generator->setOutputChannel(ui.outputChannel());
    }

    if (ui.amplitudeSlider.handle()) {
      generator->setAmplitude(ui.amplitudeSlider());
    }
  }

  std::weak_ptr<T> _generator;

protected:
  struct Ui {
    Imw::CheckBox inputEnabled{"Input"};
    Imw::CheckBox outputEnabled{"Output"};
    Imw::SpinBox<int> inputChannel{"##"};
    Imw::SpinBox<int> outputChannel{"##"};
    Imw::Slider<int> amplitudeSlider{"Amplitude %"};

    Ui() {
      inputChannel.setValueLimits({0, 95});
      outputChannel.setValueLimits({0, 95});
      amplitudeSlider.setValueLimits({1, 95});
      amplitudeSlider.setValue(1);
    }
  };
  Ui ui;
};
