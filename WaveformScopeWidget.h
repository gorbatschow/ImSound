#pragma once
#include <RtSoundClient.h>
#include <atomic>
#include <implot.h>
#include <imw.h>
#include <string>

class WaveformScopeWidget : public RtSoundClient {
public:
  WaveformScopeWidget(const std::string &name,
                      const ImVec4 &color = {0.0, 0.0, 1.0, 1.0});
  ~WaveformScopeWidget() = default;

  void paint();
  void plot();

  inline void setUpdateInterval(int interval) {
    _updateInterval.exchange(interval);
    _intervalCounter.exchange(0);
  }

  inline const std::string &label() const { return ui.enabledCheck.label(); }
  inline const ImVec4 &color() const { return _color; }

  inline bool enabled() const { return _enabled.load(); }
  inline bool isInput() const { return _isInput.load(); }
  inline bool isOutput() const { return !(_isInput.load()); }
  inline int channel() const { return _channel.load(); }
  inline int memory() const { return _memory.load(); }

  inline float rangeY() const { return 1.0f / float(ui.rangeSlider.value()); };
  inline float rangeX() const { return float(_dataSize.load()); }
  inline float rangeXB() const { return float(streamSetup().bufferFrames()); }

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

protected:
  virtual void applyStreamConfig(const RtSoundSetup &setup) override;
  virtual void streamDataReady(const RtSoundData &data) override;

private:
  inline static constexpr int PlotMaxPts{1024};
  inline static constexpr int StemMaxPts{128};
  const ImVec4 _color{};

  std::vector<float> _xData, _yData;
  std::vector<float> _xDataPlt, _yDataPlt;

  std::atomic_int _dataSize{};
  std::atomic_bool _enabled{};
  std::atomic_bool _isInput{};
  std::atomic_int _channel{};
  std::atomic_int _memory{};
  std::atomic_int _updateInterval{};
  std::atomic_int _intervalCounter{};

  void adjustDataSize();

  struct Ui {
    Imw::CheckBox enabledCheck;
    Imw::ComboBox<bool> sourceCombo;
    Imw::SpinBox<int> channelSpin;
    Imw::SpinBox<int> memorySpin;
    Imw::Slider<float> rangeSlider;

    Ui(const std::string &name) {
      enabledCheck.setLabel("Scope " + name);
      enabledCheck.setWidth(100.f);

      sourceCombo.setLabel("Source " + name);
      sourceCombo.setWidth(100.f);
      sourceCombo.setSameLine(true);
      sourceCombo.setSameLineSpacing(25.0f);
      sourceCombo.setValueList({{false, "Output"}, {true, "Input"}});

      channelSpin.setLabel("Channel " + name);
      channelSpin.setWidth(100.f);
      channelSpin.setSameLine(true);
      channelSpin.setSameLineSpacing(25.0f);
      channelSpin.setValueLimits({0, 100});

      memorySpin.setLabel("Memory " + name);
      memorySpin.setWidth(100.0f);
      memorySpin.setSameLine(true);
      memorySpin.setSameLineSpacing(25.0f);
      memorySpin.setValueLimits({1, 100});

      rangeSlider.setLabel("Range " + name);
      rangeSlider.setSameLine(true);
      rangeSlider.setWidth(-70.0f);
      rangeSlider.setSameLineSpacing(25.0f);
      rangeSlider.setValueLimits({1, 100});
      rangeSlider.setTextFormat("%.1f");
    }
  };
  Ui ui;
};
