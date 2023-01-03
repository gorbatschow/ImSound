#pragma once
#include "SoundGeneratorWidget.h"
#include <RtSoundNoiseGen.h>

class NoiseGeneratorWidget : public SoundGeneratorWidget<RtSoundNoiseGen> {
public:
  NoiseGeneratorWidget(std::weak_ptr<RtSoundNoiseGen> generator_)
      : SoundGeneratorWidget<RtSoundNoiseGen>(generator_) {}

  ~NoiseGeneratorWidget() override = default;
};
