#include "WaveformPlotWidget.h"
#include <implot.h>
#include <mutex>
#include <numeric>

WaveformPlotWidget::WaveformPlotWidget() {}

void WaveformPlotWidget::paint() {
  auto &scopeA{*_scopeA.get()};
  auto &scopeB{*_scopeB.get()};
  auto &scopeC{*_scopeC.get()};

  scopeA.paint();
  scopeB.paint();
  scopeC.paint();

  ui.updateIntervalEdit.paint();
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

  if (ui.updateIntervalEdit.handle()) {
    scopeA.setUpdateInterval(ui.updateIntervalEdit());
    scopeB.setUpdateInterval(ui.updateIntervalEdit());
    scopeC.setUpdateInterval(ui.updateIntervalEdit());
  }

  const float xRange{[&]() {
    if (_scopeA->enabled() && _scopeB->enabled()) {
      return std::max(scopeA.rangeX(), scopeB.rangeX());
    } else if (_scopeA->enabled() && !_scopeB->enabled()) {
      return float(scopeA.rangeX());
    } else if (!_scopeA->enabled() && _scopeB->enabled()) {
      return float(scopeB.rangeX());
    }
    return float(scopeA.rangeX());
  }()};
  const float xbRange{float(scopeA.rangeXB())};

  const auto flags_plt{ImPlotFlags_NoTitle | ImPlotFlags_NoLegend |
                       ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect};

  if (ImPlot::BeginPlot("Waveform", {-1, -1}, flags_plt)) {

    // Setup axes
    // ------------------------------------------------------------------------
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

    const auto flags_y3{ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickMarks |
                        ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoMenus |
                        ImPlotAxisFlags_NoSideSwitch |
                        ImPlotAxisFlags_NoHighlight | ImPlotAxisFlags_Lock};

    // Setup X1
    ImPlot::SetupAxis(ImAxis_X1, "Samples", flaxs_x1);

    // Setup Y1
    ImPlot::PushStyleColor(ImPlotCol_AxisText, scopeA.color());
    ImPlot::SetupAxis(ImAxis_Y1, scopeA.label().c_str(), flags_y1);
    ImPlot::PopStyleColor();

    // Setup Y2
    ImPlot::PushStyleColor(ImPlotCol_AxisText, scopeB.color());
    ImPlot::SetupAxis(ImAxis_Y2, scopeB.label().c_str(), flags_y2);
    ImPlot::PopStyleColor();

    // Setup Y3
    ImPlot::SetupAxis(ImAxis_Y3, scopeC.label().c_str(), flags_y3);

    // X Axis Limits
    // -------------------------------------------------------------------------
    ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f, xRange, ImGuiCond_Once);

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
    ImPlot::SetupAxisLimits(ImAxis_Y3, -1.0f, +1.0f, ImGuiCond_Once);

    if (_scopeA->enabled() && _scopeB->enabled() && _scopeC->enabled()) {

      ImPlot::SetupAxisLimits(ImAxis_Y1, -_scopeA->rangeY() * 5.0f,
                              _scopeA->rangeY(), ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y2, -_scopeB->rangeY(),
                              _scopeB->rangeY() * 5.0f, ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y3, -_scopeC->rangeY() * 3.0f,
                              _scopeC->rangeY() * 3.0f, ImPlotCond_Always);

    } else if (_scopeA->enabled() && _scopeB->enabled()) {

      ImPlot::SetupAxisLimits(ImAxis_Y1, -_scopeA->rangeY() * 3.0f,
                              _scopeA->rangeY(), ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y2, -_scopeB->rangeY(),
                              _scopeB->rangeY() * 3.0f, ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y3, -_scopeC->rangeY(), _scopeC->rangeY(),
                              ImPlotCond_Always);

    } else {

      ImPlot::SetupAxisLimits(ImAxis_Y1, -_scopeA->rangeY(), _scopeA->rangeY(),
                              ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y2, -_scopeB->rangeY(), _scopeB->rangeY(),
                              ImPlotCond_Always);

      ImPlot::SetupAxisLimits(ImAxis_Y3, -_scopeC->rangeY(), _scopeC->rangeY(),
                              ImPlotCond_Always);
    }

    // Plot
    // -------------------------------------------------------------------------

    ImPlot::SetAxis(ImAxis_Y1);
    _scopeA->plot();
    ImPlot::SetAxis(ImAxis_Y2);
    _scopeB->plot();
    ImPlot::SetAxis(ImAxis_Y3);
    _scopeC->plot();

    _axisX = {ImPlot::GetPlotLimits(ImAxis_X1), xRange, xbRange};

    ImPlot::EndPlot();
  }
}
