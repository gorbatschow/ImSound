#pragma once
#include "ImSoundWaveformAxis.h"
#include "ImSoundWaveformScopeWidget.h"
#include <RtSoundClient.h>
#include <atomic>
#include <imw.h>
#include <vector>

namespace ImSound {
class WaveformPlotWidget : public RtSound::Client
{
public:
  // Constructor
  WaveformPlotWidget();

  // Destructor
  ~WaveformPlotWidget() = default;

  // Sound Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load Widget State
  void loadWidgetState();

  // Paint
  void paint();

  inline std::weak_ptr<WaveformScopeWidget> scopeA() { return _scopeA; }
  inline std::weak_ptr<WaveformScopeWidget> scopeB() { return _scopeB; }
  inline std::weak_ptr<WaveformScopeWidget> scopeC() { return _scopeC; }

private:
  inline float get_xRange() const {
    if (_scopeA->enabled() && _scopeB->enabled()) {
      return std::max(_scopeA->rangeX(), _scopeB->rangeX());
    } else if (_scopeA->enabled() && !_scopeB->enabled()) {
      return _scopeA->rangeX();
    } else if (!_scopeA->enabled() && _scopeB->enabled()) {
      return _scopeB->rangeX();
    }
    return _scopeA->rangeX();
  }

  inline float get_xBufferRange() const { return float(_scopeA->rangeXB()); }

  std::shared_ptr<WaveformScopeWidget> _scopeA{
      new WaveformScopeWidget("A", {1.0, 1.0, 0.0, 1.0})};
  std::shared_ptr<WaveformScopeWidget> _scopeB{
      new WaveformScopeWidget("B", {0.0, 1.0, 1.0, 1.0})};
  std::shared_ptr<WaveformScopeWidget> _scopeC{
      new WaveformScopeWidget("C", {1.0, 0.0, 1.0, 1.0})};
  WaveformAxisX _axisX;

  struct Ui {
    Imw::SpinBox<int> updateIntervalEdit{"Update Interval"};
    Imw::Label xAxisLabel{"X AXIS"};
    Imw::CheckBox invertXCheck{"Invert"};
    Imw::Button maxWidthBtn{"Max"};
    Imw::Button fitWidthBtn{"Fit"};
    Imw::Button headBtn{"Head"};
    Imw::Button centerBtn{"Center"};
    Imw::Button tailBtn{"Tail"};
    Imw::Button oneWidthBtn{"1/1"};
    Imw::Button halfWidthBtn{"1/2"};
    Imw::Button quartWidthBtn{"1/4"};
    Imw::Button eighthWidthBtn{"1/8"};

    Ui() {
      updateIntervalEdit.setWidth(100.0f);
      updateIntervalEdit.setValueLimits({1, 100});
      xAxisLabel.setSameLine(true);
      xAxisLabel.setSameLineSpacing(ImGuiStyle().ItemSpacing.x * 5);
      invertXCheck.setSameLine(true);
      maxWidthBtn.setSameLine(true);
      fitWidthBtn.setSameLine(true);
      oneWidthBtn.setSameLine(true);
      halfWidthBtn.setSameLine(true);
      quartWidthBtn.setSameLine(true);
      eighthWidthBtn.setSameLine(true);
      centerBtn.setSameLine(true);
      headBtn.setSameLine(true);
      tailBtn.setSameLine(true);
    }
  };
  Ui ui;
};
} // namespace ImSound
