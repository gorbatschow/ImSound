#pragma once
#include <RtSoundNoiseGen.h>
#include <imgui_internal.h>
#include <imw.h>

template <class T> class SoundGeneratorWidget : public RtSoundClient {
public:
  SoundGeneratorWidget(std::weak_ptr<T> generator_) : _generator(generator_) {
    const auto generator{_generator.lock()};
    assert(generator);
  }

  virtual ~SoundGeneratorWidget() = default;

  virtual void paint() {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    // Paint
    ImGui::BeginGroup();
    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
    ui.toInput.paint();
    ImGui::PopItemWidth();
    ui.channel.paint();
    ImGui::PopItemWidth();
    ImGui::EndGroup();
    ui.enabled.paint();
    ui.amplitudeSlider.paint();

    // Handle
    if (ui.enabled.handle()) {
      generator->setEnabled(ui.enabled());
    }

    if (ui.toInput.handle()) {
      generator->setSendToInput(ui.toInput());
    }

    if (ui.channel.handle()) {
      generator->setChannel(ui.channel());
    }

    if (ui.amplitudeSlider.handle()) {
      generator->setAmplitudePercent(ui.amplitudeSlider());
    }
  }

protected:
  void applyStreamConfig(const RtSoundSetup &setup) override {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    generator->setAmplitudePercent(ui.amplitudeSlider());
  }

  std::weak_ptr<T> _generator;

  struct Ui {
    class InOutSwitch : public Imw::ComboBox<bool> {
    public:
      InOutSwitch() {
        Imw::ComboBox<bool>::setValueList({{false, "Output"}, {true, "Input"}});
      }
    };

    Imw::CheckBox enabled{"Enabled"};
    InOutSwitch toInput{};
    Imw::SpinBox<int> channel{"##"};
    Imw::Slider<float> amplitudeSlider{"Amplitude %"};

    Ui() {
      channel.setSameLine(true);
      channel.setValueLimits({0, 100});
      enabled.setSameLine(true);
      amplitudeSlider.setValueLimits({1e-3f, 1e2f});
      amplitudeSlider.setValue(1.0f);
      amplitudeSlider.setTextFormat("%.2f");
    }
  };
  Ui ui;
};
