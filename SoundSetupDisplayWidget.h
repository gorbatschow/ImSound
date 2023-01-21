#pragma once
#include <RtSoundClient.h>

class SoundSetupDisplayWidget : public RtSoundClient {
public:
  SoundSetupDisplayWidget() = default;
  ~SoundSetupDisplayWidget() = default;

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  void paint();

protected:
  virtual void applyStreamConfig(const RtSoundSetup &setup) override;

private:
  RtSoundSetup _setup;

  void appendRow(const std::string &param = {}, const std::string &value = {});
};
