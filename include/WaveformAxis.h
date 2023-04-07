#pragma once
#include <implot.h>
#include <vector>

class WaveformAxisX {
public:
  WaveformAxisX() = default;
  WaveformAxisX(const ImPlotRect &rect, float dataRange, float bufferRange)
      : _lims{float(rect.X.Min), float(rect.X.Max)},
        _center{(_lims.second + _lims.first) / 2.0f}, _plotRange{_lims.second -
                                                                 _lims.first},
        _dataRange{dataRange}, _bufferRange{bufferRange} {}

  inline const std::pair<float, float> &lims() const { return _lims; }
  inline const float center() const { return _center; }
  inline const float plotRange() const { return _plotRange; }

  inline float limsPlotMin() const { return _lims.first; }
  inline float limsPlotMax() const { return _lims.second; }

  inline float limsDataMin() const { return 0.0f; }
  inline float limsDataMax() const { return _dataRange - 1.0f; }

  inline float limsMaxMin() const { return -0.25f * _dataRange; }
  inline float limsMaxMax() const { return +1.25f * _dataRange; }

  inline float limsFitMin() const { return 0.0f; }
  inline float limsFitMax() const { return _dataRange - 1.0f; }

  inline float limsHeadMin() const { return -0.25f * _plotRange; }
  inline float limsHeadMax() const { return +0.75 * _plotRange; }

  inline float limsCenterMin() const {
    return -0.5f * _plotRange + _dataRange / 2.0f;
  }
  inline float limsCenterMax() const {
    return +0.5f * _plotRange + _dataRange / 2.0f;
  }

  inline float limsTailMin() const { return _dataRange - 0.75f * _plotRange; }
  inline float limsTailMax() const { return _dataRange + 0.25f * _plotRange; }

  inline float limsOneMin() const { return _center - _bufferRange / 2.0f; }
  inline float limsOneMax() const { return _center + _bufferRange / 2.0f; }

  inline float limsHalfMin() const { return _center - _bufferRange / 4.0f; }
  inline float limsHalfMax() const { return _center + _bufferRange / 4.0f; }

  inline float limsQuartMin() const { return _center - _bufferRange / 8.0f; }
  inline float limsQuartMax() const { return _center + _bufferRange / 8.0f; }

  inline float limsEightMin() const { return _center - _bufferRange / 16.0f; }
  inline float limsEightMax() const { return _center + _bufferRange / 16.0f; }

  inline bool dataInPlotRange() const {
    return limsPlotMin() < limsDataMin() && limsPlotMax() > limsDataMax();
  }

private:
  std::pair<float, float> _lims{};
  float _center{};
  float _plotRange{};
  float _dataRange{};
  float _bufferRange{};
};
