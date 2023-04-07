#pragma once
#include "ImSoundGeneratorWidget.h"
#include <RtSoundNoiseGen.h>

namespace ImSound {
class NoiseGeneratorWidget : public GeneratorWidget<RtSound::NoiseGen>
{
public:
  NoiseGeneratorWidget(std::weak_ptr<RtSound::NoiseGen> generator_)
      : GeneratorWidget<RtSound::NoiseGen>(generator_) {}

  ~NoiseGeneratorWidget() override = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }
};
} // namespace ImSound
