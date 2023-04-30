#pragma once
#include <RtSoundClient.h>

namespace ImSound {
class SetupDisplayWidget : public RtSound::Client
{
public:
  SetupDisplayWidget();
  ~SetupDisplayWidget() = default;

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  void paint();

private:
  virtual void applyStreamConfig(const RtSound::StreamSetup &setup) override;
  void appendRow(const std::string &param = {}, const std::string &value = {});

  std::unique_ptr<RtSound::StreamSetup> _setup;
};
} // namespace ImSound
