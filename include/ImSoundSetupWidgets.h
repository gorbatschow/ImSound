#pragma once
#include <RtAudio.h>
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <imw.h>
#include <math.h>
#include <memory.h>

namespace ImSound {
// AudioApiCombo
// -----------------------------------------------------------------------------
class AudioApiCombo : public ImWrap::ComboBox<RtAudio::Api>
{
public:
  AudioApiCombo() {
    _label = "Audio API";
    std::vector<RtAudio::Api> rtAPIs;
    RtAudio::getCompiledApi(rtAPIs);
    _valueList.resize(rtAPIs.size());
    std::transform(
        rtAPIs.begin(), rtAPIs.end(), _valueList.begin(), [](RtAudio::Api api) {
          return ImWrap::NamedValue{api, RtAudio::getApiDisplayName(api)};
        });
    if (_valueList.size() > 0) {
      //_currIndex = 0;
    }
  }

protected:
};

// SampleRateCombo
// -----------------------------------------------------------------------------
class SampleRateCombo : public ImWrap::ComboBox<int>
{
public:
  SampleRateCombo() {
    _label = "Sample Rate [kHz]";
    _valueList = {{192000, "192"},
                  {96000, "96"},
                  {48000, "48"},
                  {44100, "44.1"},
                  {24000, "24"},
                  {12000, "12"}};
    _currIndex = 0;
  }
};

// BufferSizeInput
// -----------------------------------------------------------------------------
class BufferSizeInput : public ImWrap::ValueElement<int>
{
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
class StreamStatusLine : public ImWrap::BasicElement, public RtSound::Client
{
public:
  StreamStatusLine() {}

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

protected:
  void paintElement() override {
    if (streamInfo().streamRunning()) {
      ImGui::Text("Trun=%.1fs", streamInfo().streamTime());
    } else {
      ImGui::Text("Stopped");
    }
  }
};

// AudioDeviceCombo
// -----------------------------------------------------------------------------
class SoundDeviceCombo : public ImWrap::ComboBox<RtAudio::DeviceInfo>,
                         public RtSound::Client
{
  using Base = ImWrap::ComboBox<RtAudio::DeviceInfo>;

public:
  enum DeviceType { AllDevices, InputDevices, OutputDevices };

  // Constructor
  SoundDeviceCombo(DeviceType t = AllDevices, bool displayAll = true)
      : _deviceType(t), _displayAll(displayAll) {
    if (t == AllDevices) {
      setLabel("Audio Device");
    } else if (t == InputDevices) {
      setLabel("Input Device");
    } else if (t == OutputDevices) {
      setLabel("Output Device");
    }
    _enabledFlag.setLabel("Enabled");
  }

  // Destructor
  virtual ~SoundDeviceCombo() = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  virtual void loadStateFromFile() override {
    Base::loadStateFromFile();
    _enabledFlag.loadStateFromFile();
  }

  bool deviceEnabled() const { return _enabledFlag.value(); }

protected:
  void paintElement() override {
    ImWrap::ComboBox<RtAudio::DeviceInfo>::paintElement();
    ImGui::SameLine();
    _enabledFlag.paint();
  }

private:
  const DeviceType _deviceType{AllDevices};
  ImWrap::CheckBox _enabledFlag;
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
class FrequencySpinBox : public ImWrap::SpinBox<int>
{
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
} // namespace ImSound
