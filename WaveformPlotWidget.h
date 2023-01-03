#pragma once
#include "WaveformAxis.h"
#include "WaveformScopeWidget.h"
#include <RtSoundClient.h>
#include <atomic>
#include <imw.h>
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
