#pragma once
#include "WaveformAxis.h"
#include "WaveformScopeWidget.h"
#include <ImWrapper.h>
#include <RtSoundClient.h>
#include <atomic>
#include <vector>

class WaveformPlotWidget : public RtSoundClient {
public:
  WaveformPlotWidget();
  ~WaveformPlotWidget() = default;

  void paint();

  inline std::weak_ptr<WaveformScopeWidget> scopeA() { return _scopeA; }
  inline std::weak_ptr<WaveformScopeWidget> scopeB() { return _scopeB; }

private:
  std::shared_ptr<WaveformScopeWidget> _scopeA{new WaveformScopeWidget("A")};
  std::shared_ptr<WaveformScopeWidget> _scopeB{new WaveformScopeWidget("B")};
  WaveformAxisX _axisX;

  struct Ui {
    ImWrap::Label xAxisLabel{"X AXIS"};
    ImWrap::CheckBox invertXCheck{"Invert"};
    ImWrap::Button maxWidthBtn{"Max"};
    ImWrap::Button fitWidthBtn{"Fit"};
    ImWrap::Button headBtn{"Head"};
    ImWrap::Button centerBtn{"Center"};
    ImWrap::Button tailBtn{"Tail"};
    ImWrap::Button oneWidthBtn{"1/1"};
    ImWrap::Button halfWidthBtn{"1/2"};
    ImWrap::Button quartWidthBtn{"1/4"};
    ImWrap::Button eighthWidthBtn{"1/8"};

    Ui() {
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
