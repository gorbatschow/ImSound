#pragma once
#include "SoundGeneratorWidget.h"
#include <RtSoundNoiseGen.h>

namespace ImSound {
class NoiseGeneratorWidget : public SoundGeneratorWidget<RtSound::NoiseGen>
{
public:
  NoiseGeneratorWidget(std::weak_ptr<RtSound::NoiseGen> generator_)
      : SoundGeneratorWidget<RtSound::NoiseGen>(generator_) {}

  ~NoiseGeneratorWidget() override = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }
};
} // namespace ImSound
