#include "WaveformScopeWidget.h"
#include "DownsampleLTTB.h"
#include <implot.h>
#include <mutex>
#include <numeric>

WaveformScopeWidget::WaveformScopeWidget(const std::string &name) : ui(name) {
  _xDataPlt.resize(PlotMaxPts);
  _xDataPlt.shrink_to_fit();
  _yDataPlt.resize(PlotMaxPts);
  _yDataPlt.shrink_to_fit();

  _enabled.exchange(ui.enabledCheck());
  _isOutput.exchange(ui.sourceCombo());
  _channel.exchange(ui.channelSpin());
  _memory.exchange(ui.memorySpin());
}

void WaveformScopeWidget::paint() {
  // Paint
  ui.enabledCheck.paint();
  ui.sourceCombo.paint();
  ui.channelSpin.paint();
  ui.memorySpin.paint();
  ui.rangeSlider.paint();

  // Handle
  if (ui.enabledCheck.handle()) {
    _enabled.exchange(ui.enabledCheck());
    std::lock_guard lock(streamData().mutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.sourceCombo.handle()) {
    _isOutput.exchange(ui.sourceCombo());
    std::lock_guard lock(streamData().mutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.channelSpin.handle()) {
    _channel.exchange(ui.channelSpin());
    std::lock_guard lock(streamData().mutex);
    std::fill(_yData.begin(), _yData.end(), 0.0);
  }

  if (ui.memorySpin.handle()) {
    _memory.exchange(ui.memorySpin());
    std::lock_guard lock(streamData().mutex);
    adjustDataSize();
  }
}

void WaveformScopeWidget::plot() {
  if (!_enabled) {
    return;
  }

  const auto xlimf{ImPlot::GetPlotLimits().X};
  const int xmin{std::clamp(int(xlimf.Min) - 1, 0, int(_xData.size()) - 1)};
  const int xmax{std::clamp(int(xlimf.Max) + 1, 0, int(_xData.size()) - 1)};
  const int npts{std::clamp(int(xmax - xmin + 1), 0, int(_xData.size()))};

  if (npts > PlotMaxPts) {
    std::lock_guard lock(streamData().mutex);
    DownsampleLTTB(&_xData[xmin], &_yData[xmin], npts, &_xDataPlt[0],
                   &_yDataPlt[0], PlotMaxPts);
    ImPlot::PlotLine(label().c_str(), &_xDataPlt[0], &_yDataPlt[0], PlotMaxPts);
  } else if (npts > StemMaxPts) {
    std::lock_guard lock(streamData().mutex);
    ImPlot::PlotLine(label().c_str(), &_xData[xmin], &_yData[xmin], npts);
  } else {
    std::lock_guard lock(streamData().mutex);
    ImPlot::PlotStems(label().c_str(), &_xData[xmin], &_yData[xmin], npts);
  }
}

void WaveformScopeWidget::applyStreamConfig(const RtSoundSetup &setup) {
  adjustDataSize();
}

void WaveformScopeWidget::streamDataReady(const RtSoundData &data) {
  if (!_enabled) {
    return;
  }

  if (!_isOutput && _channel < data.inputsN()) {
    std::lock_guard lock(data.mutex);
    std::rotate(_yData.rbegin(), _yData.rbegin() + data.framesN(),
                _yData.rend());
    data.rcopyInput(_channel, _yData.data());
  } else if (_isOutput && _channel < data.outputsN()) {
    std::lock_guard lock(data.mutex);
    data.rcopyOutput(_channel, _yData.data());
    std::rotate(_yData.rbegin(), _yData.rbegin() + data.framesN(),
                _yData.rend());
  }
}

void WaveformScopeWidget::adjustDataSize() {
  _dataSize.exchange(streamSetup().bufferFrames() * ui.memorySpin());
  _xData.resize(_dataSize);
  _xData.shrink_to_fit();
  _yData.resize(_dataSize);
  _yData.shrink_to_fit();
  std::iota(_xData.begin(), _xData.end(), 0);
}
