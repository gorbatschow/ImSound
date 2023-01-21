#pragma once
#include <RtAudio.h>
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <imw.h>
#include <math.h>
#include <memory.h>

// AudioApiCombo
// -----------------------------------------------------------------------------
class AudioApiCombo : public Imw::ComboBox<RtAudio::Api> {
public:
  AudioApiCombo() {
    _label = "Audio API";
    std::vector<RtAudio::Api> rtAPIs;
    RtAudio::getCompiledApi(rtAPIs);
    _valueList.resize(rtAPIs.size());
    std::transform(
        rtAPIs.begin(), rtAPIs.end(), _valueList.begin(), [](RtAudio::Api api) {
          return Imw::NamedValue{api, RtAudio::getApiDisplayName(api)};
        });
  }

protected:
};

// SampleRateCombo
// -----------------------------------------------------------------------------
class SampleRateCombo : public Imw::ComboBox<int> {
public:
  SampleRateCombo() {
    _label = "Sample Rate (Hz)";
    _valueList = {{192000, "192k"}, {96000, "96k"}, {48000, "48k"},
                  {44100, "44.1k"}, {24000, "24k"}, {12000, "12k"}};
  }
};

// BufferSizeInput
// -----------------------------------------------------------------------------
class BufferSizeInput : public Imw::ValueElement<int> {
public:
  BufferSizeInput() {
    _label = "Buffer Size";
    _value = 1024;
  }

  void paintElement() override {
    if (ImGui::InputInt(_label.c_str(), &_powerOfTwo, 1, 1)) {
      _powerOfTwo = _powerOfTwo < 1 ? 1 : _powerOfTwo;
      _powerOfTwo = _powerOfTwo > 20 ? 20 : _powerOfTwo;
      _value = (int(1)) << _powerOfTwo;
    }
    ImGui::SameLine();
    ImGui::Text("%d", _value);
  }

  // Set Value
  virtual void setValue(const int &value, int) override {
    _powerOfTwo = int(std::log2(double(value)));
    _value = (int(1)) << _powerOfTwo;
  }

private:
  int _powerOfTwo{10};
};

// StreamStatusLine
// -----------------------------------------------------------------------------
class StreamStatusLine : public Imw::BasicElement {

public:
  StreamStatusLine(std::weak_ptr<RtSoundIO> io) : _io{io} {}

protected:
  void paintElement() override {
    const auto soundIO{_io.lock()};
    assert(soundIO);

    if (soundIO->streamInfo().streamRunning()) {
      ImGui::Text("Trun=%.1fs", soundIO->streamInfo().streamTime());
    } else {
      ImGui::Text("Stopped");
    }
  }

private:
  std::weak_ptr<RtSoundIO> _io;
};

// AudioDeviceCombo
// -----------------------------------------------------------------------------
class SoundDeviceCombo : public Imw::ComboBox<RtAudio::DeviceInfo>,
                         public RtSoundClient {
public:
  enum DeviceType { AllDevices, InputDevices, OutputDevices };

  // Constructor
  SoundDeviceCombo(DeviceType t = AllDevices, bool displayAll = true)
      : _deviceType(t), _displayAll(displayAll) {
    if (t == AllDevices) {
      _enabledFlag.setLabel("Audio Device");
    } else if (t == InputDevices) {
      _enabledFlag.setLabel("Input Device");
    } else if (t == OutputDevices) {
      _enabledFlag.setLabel("Output Device");
    }
  }

  // Destructor
  virtual ~SoundDeviceCombo() = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  bool deviceEnabled() const { return _enabledFlag.value(); }

protected:
  void paintElement() override {
    Imw::ComboBox<RtAudio::DeviceInfo>::paintElement();
    ImGui::SameLine();
    _enabledFlag.paint();
  }

private:
  const DeviceType _deviceType{AllDevices};
  Imw::CheckBox _enabledFlag;
  bool _displayAll{};

  void
  updateSoundDevices(const std::vector<RtAudio::DeviceInfo> &devices) override {
    const std::string currName{isCurrentValid() ? currentValue().name() : ""};
    _currIndex = -1;
    _valueList.clear();
    _valueList.reserve(devices.size());
    for (auto &device : devices) {
      if (_deviceType == DeviceType::AllDevices || _displayAll) {
        _valueList.push_back({device, device.name});
      } else if (_deviceType == InputDevices &&
                 (device.inputChannels > 0 || device.duplexChannels > 0)) {
        _valueList.push_back({device, device.name});
      } else if (_deviceType == OutputDevices &&
                 (device.outputChannels > 0 || device.duplexChannels > 0)) {
        _valueList.push_back({device, device.name});
      }
    }
    _valueList.shrink_to_fit();

    if (!_valueList.empty()) {
      setCurrentName(currName);
    }
    if (!_valueList.empty() && _currIndex < 0) {
      _currIndex = 0;
    }
  }
};

// FrequencySpinBox
// -----------------------------------------------------------------------------
class FrequencySpinBox : public Imw::SpinBox<int> {
public:
  FrequencySpinBox() {}

  void paintElement() override {
    SpinBox::paintElement();
    ImGui::SameLine();
    if (_value > 0) {
      ImGui::Text("%.2f Hz", _df * _value);
    } else {
      ImGui::Text("Wideband");
    }
  }

private:
  double _df{0.0f};
};
