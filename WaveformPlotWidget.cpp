#include "WaveformPlotWidget.h"
#include <implot.h>
#include <mutex>
#include <numeric>

WaveformPlotWidget::WaveformPlotWidget() {}

void WaveformPlotWidget::paint() {
  auto &scopeA{*_scopeA.get()};
  auto &scopeB{*_scopeB.get()};

  scopeA.paint();
  scopeB.paint();
  ui.xAxisLabel.paint();
  ui.invertXCheck.paint();
  ui.maxWidthBtn.paint();
  ui.fitWidthBtn.paint();
  ui.headBtn.paint();
  ui.centerBtn.paint();
  ui.tailBtn.paint();
  ui.oneWidthBtn.paint();
  ui.halfWidthBtn.paint();
  ui.quartWidthBtn.paint();
  ui.eighthWidthBtn.paint();

  const float bufferRange{float(scopeA.bufferSize())};
  const float dataRange{[&]() {
    if (_scopeA->enabled() && _scopeB->enabled()) {
      return float(std::max(scopeA.dataSize(), scopeB.dataSize()));
    } else if (_scopeA->enabled() && !_scopeB->enabled()) {
      return float(scopeA.dataSize());
    } else if (!_scopeA->enabled() && _scopeB->enabled()) {
      return float(scopeB.dataSize());
    }
    return float(scopeA.dataSize());
  }()};

  const auto flags_plt{ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                       ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect};

  if (ImPlot::BeginPlot("Waveform", {-1, -1}, flags_plt)) {
    const auto flaxs_x1{[&]() {
      ImPlotAxisFlags flags = ImPlotAxisFlags_NoSideSwitch |
                              ImPlotAxisFlags_NoHighlight |
                              ImPlotAxisFlags_NoMenus;
      if (ui.invertXCheck()) {
        flags |= ImPlotAxisFlags_Invert;
      }
      return flags;
    }()};

    const auto flags_y1{ImPlotAxisFlags_NoGridLines |
                        ImPlotAxisFlags_NoSideSwitch |
                        ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_NoMenus};

    const auto flags_y2{flags_y1 | ImPlotAxisFlags_Opposite};

    ImPlot::SetupAxis(ImAxis_X1, "Buffer Samples", flaxs_x1);
    ImPlot::SetupAxis(ImAxis_Y1, scopeA.label().c_str(), flags_y1);
    ImPlot::SetupAxis(ImAxis_Y2, scopeB.label().c_str(), flags_y2);

    // X Axis Limits
    // -------------------------------------------------------------------------
    ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f, dataRange - 1.0f, ImGuiCond_Once);

    if (ui.maxWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsMaxMin(),
                              _axisX.limsMaxMax(), ImGuiCond_Always);
    }

    if (ui.fitWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsFitMin(),
                              _axisX.limsFitMax(), ImGuiCond_Always);
    }

    if (ui.oneWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsOneMin(),
                              _axisX.limsOneMax(), ImGuiCond_Always);
    }

    if (ui.halfWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsHalfMin(),
                              _axisX.limsHalfMax(), ImGuiCond_Always);
    }

    if (ui.quartWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsQuartMin(),
                              _axisX.limsQuartMax(), ImGuiCond_Always);
    }

    if (ui.eighthWidthBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsEightMin(),
                              _axisX.limsEightMax(), ImGuiCond_Always);
    }

    if (ui.headBtn.handle()) {
      if (_axisX.dataInPlotRange()) {
        ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsCenterMin(),
                                _axisX.limsCenterMax(), ImGuiCond_Always);
      } else {
        ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsHeadMin(),
                                _axisX.limsHeadMax(), ImGuiCond_Always);
      }
    }

    if (ui.centerBtn.handle()) {
      ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsCenterMin(),
                              _axisX.limsCenterMax(), ImGuiCond_Always);
    }

    if (ui.tailBtn.handle()) {
      if (_axisX.dataInPlotRange()) {
        ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsCenterMin(),
                                _axisX.limsCenterMax(), ImGuiCond_Always);
      } else {
        ImPlot::SetupAxisLimits(ImAxis_X1, _axisX.limsTailMin(),
                                _axisX.limsTailMax(), ImGuiCond_Always);
      }
    }

    // Y Axis Limits
    // -------------------------------------------------------------------------
    ImPlot::SetupAxisLimits(ImAxis_Y1, -1.0f, +1.0f, ImGuiCond_Once);
    ImPlot::SetupAxisLimits(ImAxis_Y2, -1.0f, +1.0f, ImGuiCond_Once);

    if (_scopeA->enabled() && _scopeB->enabled()) {
      ImPlot::SetupAxisLimits(ImAxis_Y1, -_scopeA->range() * 3.0f,
                              _scopeA->range(), ImPlotCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y2, -_scopeB->range(),
                              _scopeB->range() * 3.0f, ImPlotCond_Always);
    } else {
      ImPlot::SetupAxisLimits(ImAxis_Y1, -_scopeA->range(), _scopeA->range(),
                              ImPlotCond_Always);
      ImPlot::SetupAxisLimits(ImAxis_Y2, -_scopeB->range(), _scopeB->range(),
                              ImPlotCond_Always);
    }

    // Plot
    // -------------------------------------------------------------------------
    ImPlot::SetAxis(ImAxis_Y1);
    _scopeA->plot();
    ImPlot::SetAxis(ImAxis_Y2);
    _scopeB->plot();

    _axisX = {ImPlot::GetPlotLimits(ImAxis_X1), dataRange, bufferRange};

    ImPlot::EndPlot();
  }
}
