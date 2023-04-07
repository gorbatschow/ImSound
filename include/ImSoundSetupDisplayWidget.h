#pragma once
#include <RtSoundClient.h>

namespace ImSound {
class SoundSetupDisplayWidget : public RtSound::Client
{
public:
  SoundSetupDisplayWidget() = default;
  ~SoundSetupDisplayWidget() = default;

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  void paint();

protected:
  virtual void applyStreamConfig(const RtSound::StreamSetup &setup) override;

private:
  RtSound::StreamSetup _setup;

  void appendRow(const std::string &param = {}, const std::string &value = {});
};
} // namespace ImSound
