#pragma once
#include <RtSoundNoiseGen.h>
#include <imgui_internal.h>
#include <imw.h>

namespace ImSound {
template<class T>
class SoundGeneratorWidget : public RtSound::Client
{
public:
  // Constructor
  SoundGeneratorWidget(std::weak_ptr<T> generator_) : _generator(generator_) {
    const auto generator{_generator.lock()};
    assert(generator);
  }

  // Destructor
  virtual ~SoundGeneratorWidget() = default;

  // Sound Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  virtual void loadWidgetState() {
    ui.toInputSwitch.loadStateFromFile();
    ui.channelEdit.loadStateFromFile();
    ui.enabledFlag.loadStateFromFile();
    ui.gateIntervalEdit.loadStateFromFile();
    ui.gateEnabledFlag.loadStateFromFile();
    ui.gateFrameEdit.loadStateFromFile();
    ui.amplitudeEdit.loadStateFromFile();

    ui.enabledFlag.trigger();
    ui.toInputSwitch.trigger();
    ui.channelEdit.trigger();
    ui.gateIntervalEdit.trigger();
    ui.gateEnabledFlag.trigger();
    ui.gateFrameEdit.trigger();
    ui.amplitudeEdit.trigger();
  }

  // Paint
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
    ui.gateIntervalEdit.paint();
    ui.gateEnabledFlag.paint();
    ui.gateFrameEdit.paint();
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
    if (ui.gateIntervalEdit.handle()) {
      generator->setGateInterval(ui.gateIntervalEdit());
    }
    if (ui.gateFrameEdit.handle()) {
      generator->setGateOpenFrame(ui.gateFrameEdit(0));
      generator->setGateFrameCount(ui.gateFrameEdit(1));
    }
    if (ui.amplitudeEdit.handle()) {
      generator->setAmplitudePercent(ui.amplitudeEdit());
    }
  }

protected:
  void applyStreamConfig(const RtSound::StreamSetup &setup) override {
    const auto generator{_generator.lock()};
    assert(generator != nullptr);

    ui.enabledFlag.trigger();
    ui.toInputSwitch.trigger();
    ui.channelEdit.trigger();
    ui.gateIntervalEdit.trigger();
    ui.gateEnabledFlag.trigger();
    ui.gateFrameEdit.trigger();
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
    Imw::SpinBox<int> gateIntervalEdit{"##"};
    Imw::CheckBox gateEnabledFlag{"Gate Interval"};
    Imw::MultiSpinBox<int> gateFrameEdit{2, "Gate Frame Open/Count"};
    Imw::Slider<float> amplitudeEdit{"Amplitude %"};

    Ui() {
      channelEdit.setSameLine(true);
      channelEdit.setValueLimits({0, 100});
      channelEdit.setSameLineSpacing(ImGuiStyle().ItemInnerSpacing.x);
      enabledFlag.setSameLine(true);
      gateIntervalEdit.setValueLimits({1, 100});
      gateFrameEdit.setValueLimits({0, 100000}, 0);
      gateFrameEdit.setValueLimits({0, 100000}, 1);
      amplitudeEdit.setValueLimits({1e-3f, 1e2f});
      amplitudeEdit.setValue(1.0f);
      amplitudeEdit.setTextFormat("%.2f");
      gateEnabledFlag.setSameLine(true);
    }
  };
  Ui ui;
};
} // namespace ImSound
