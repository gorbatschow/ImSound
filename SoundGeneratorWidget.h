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
    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
    ui.toInputSwitch.paint();
    ImGui::PopItemWidth();
    ui.channelEdit.paint();
    ImGui::PopItemWidth();
    ui.enabledFlag.paint();
    ui.gateEdit.paint();
    ui.gateEnabledFlag.paint();
    ui.amplitudeEdit.paint();

    // Handle
    if (ui.enabledFlag.handle()) {
      generator->setEnabled(ui.enabledFlag());
    }
    if (ui.toInputSwitch.handle()) {
      generator->setSendToInput(ui.toInputSwitch());
    }
    if (ui.channelEdit.handle()) {
      generator->setChannel(ui.channelEdit());
    }
    if (ui.gateEnabledFlag.handle()) {
      generator->setGateEnabled(ui.gateEnabledFlag());
    }
    if (ui.gateEdit.handle()) {
      generator->setGateOpenFrame(ui.gateEdit(0));
      generator->setGateFrameCount(ui.gateEdit(1));
    }
    if (ui.amplitudeEdit.handle()) {
      generator->setAmplitudePercent(ui.amplitudeEdit());
    }
  }

protected:
  void applyStreamConfig(const RtSoundSetup &setup) override {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    ui.enabledFlag.trigger();
    ui.toInputSwitch.trigger();
    ui.channelEdit.trigger();
    ui.gateEnabledFlag.trigger();
    ui.gateEdit.trigger();
    ui.amplitudeEdit.trigger();
  }

  std::weak_ptr<T> _generator;

  struct Ui {
    class InOutSwitch : public Imw::ComboBox<bool> {
    public:
      InOutSwitch() {
        Imw::ComboBox<bool>::setValueList({{false, "Output"}, {true, "Input"}});
      }
    };

    Imw::CheckBox enabledFlag{"Enabled"};
    InOutSwitch toInputSwitch{};
    Imw::SpinBox<int> channelEdit{"##"};
    Imw::MultiSpinBox<int> gateEdit{2};
    Imw::CheckBox gateEnabledFlag{"Gate Frame Open/Count"};
    Imw::Slider<float> amplitudeEdit{"Amplitude %"};

    Ui() {
      channelEdit.setSameLine(true);
      channelEdit.setValueLimits({0, 100});
      channelEdit.setSameLineSpacing(ImGuiStyle().ItemInnerSpacing.x);
      enabledFlag.setSameLine(true);
      amplitudeEdit.setValueLimits({1e-3f, 1e2f});
      amplitudeEdit.setValue(1.0f);
      amplitudeEdit.setTextFormat("%.2f");
      gateEnabledFlag.setSameLine(true);
    }
  };
  Ui ui;
};
