#include "WaveformScopeWidget.h"
#include "DownsampleLTTB.h"
#include <algorithm>
#include <implot.h>
#include <mutex>
#include <numeric>

WaveformScopeWidget::WaveformScopeWidget(const std::string &name,
                                         const ImVec4 &color)
    : ui(name), _color(color) {
  _xDataPlt.resize(PlotMaxPts);
  _yDataPlt.resize(PlotMaxPts);

  _enabled.exchange(ui.enabledCheck());
  _isInput.exchange(ui.sourceCombo());
  _channel.exchange(ui.channelSpin());
  _memory.exchange(ui.memorySpin());
}

void WaveformScopeWidget::paint() {
  // Paint
  ImGui::PushStyleColor(ImGuiCol_CheckMark, _color);
  ui.enabledCheck.paint();
  ui.sourceCombo.paint();
  ui.channelSpin.paint();
  ui.memorySpin.paint();
  ui.rangeSlider.paint();
  ImGui::PopStyleColor();

  // Handle
  if (ui.enabledCheck.handle()) {
    _enabled.exchange(ui.enabledCheck());
    std::lock_guard lock(clientMutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.sourceCombo.handle()) {
    _isInput.exchange(ui.sourceCombo());
    std::lock_guard lock(clientMutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.channelSpin.handle()) {
    _channel.exchange(ui.channelSpin());
    std::lock_guard lock(clientMutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.memorySpin.handle()) {
    _memory.exchange(ui.memorySpin());
    std::lock_guard lock(clientMutex);
    adjustDataSize();
  }
}

void WaveformScopeWidget::plot() {
  if (!_enabled) {
    return;
  }

  const auto dataSize{_dataSize.load()};
  const auto xlimf{ImPlot::GetPlotLimits().X};
  const int xmin{std::clamp(int(xlimf.Min) - 1, 0, dataSize - 1)};
  const int xmax{std::clamp(int(xlimf.Max) + 1, 0, dataSize - 1)};
  const int npts{std::clamp(int(xmax - xmin + 1), 0, dataSize)};

  const auto intervalCounter{_intervalCounter.load()};
  if (intervalCounter == 0) {
    std::lock_guard lock(clientMutex);
    if (npts > PlotMaxPts) {
      DownsampleLTTB(&_xData[xmin], &_yData[xmin], npts, &_xDataPlt[0],
                     &_yDataPlt[0], PlotMaxPts);
    } else {
      std::copy(&_xData[xmin], &_xData[xmin] + npts, &_xDataPlt[0]);
      std::copy(&_yData[xmin], &_yData[xmin] + npts, &_yDataPlt[0]);
    }
  }

  ImPlot::PushStyleColor(ImPlotCol_Line, _color);
  {
    std::lock_guard lock(clientMutex);
    if (npts > StemMaxPts) {
      ImPlot::PlotLine(label().c_str(), &_xDataPlt[0], &_yDataPlt[0],
                       std::min(npts, PlotMaxPts));
    } else {
      ImPlot::PlotStems(label().c_str(), &_xDataPlt[0], &_yDataPlt[0],
                        std::min(npts, PlotMaxPts));
    }
  }
  ImPlot::PopStyleColor();
}

void WaveformScopeWidget::applyStreamConfig(const RtSoundSetup &setup) {
  adjustDataSize();
}

void WaveformScopeWidget::streamDataReady(const RtSoundData &data) {
  if (!_enabled) {
    return;
  }

  const auto isInput{_isInput.load()};
  const auto channel{_channel.load()};

  if (!data.hasChannel(isInput, channel)) {
    return;
  }

  const auto buffer{isInput ? data.inputBuffer(channel)
                            : data.outputBuffer(channel)};
  {
    std::lock_guard lock(clientMutex);
    std::copy(buffer, buffer + data.framesN(), _yData.rbegin());
    std::rotate(_yData.rbegin(), _yData.rbegin() + data.framesN(),
                _yData.rend());
  }

  const auto interval{_updateInterval.load()};
  if (++_intervalCounter >= interval) {
    _intervalCounter.exchange(0);
  }
}

void WaveformScopeWidget::adjustDataSize() {
  _dataSize.exchange(streamSetup().bufferFrames() * ui.memorySpin());
  _xData.resize(_dataSize);
  _yData.resize(_dataSize);
  std::iota(_xData.begin(), _xData.end(), 0);
}
