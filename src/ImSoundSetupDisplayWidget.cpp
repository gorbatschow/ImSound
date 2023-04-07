#include "ImSoundSetupDisplayWidget.h"
#include <imgui.h>

namespace ImSound {
void SetupDisplayWidget::paint() {
  if (ImGui::BeginTable("Setup Table", 2)) {
    appendRow("API", _setup.soundApiName());
    appendRow("Sample Rate", std::to_string(_setup.sampleRate()));
    appendRow("Buffer Frames", std::to_string(_setup.bufferFrames()));
    appendRow();
    appendRow("Input Enabled", _setup.inputEnabled() ? "True" : "False");
    appendRow("Input Device", _setup.inputDeviceName());
    appendRow("Input Channels", std::to_string(_setup.inputChannels()));
    appendRow("Input First Channel",
              std::to_string(_setup.inputFirstChannel()));
    appendRow();
    appendRow("Output Enabled", _setup.outputEnabled() ? "True" : "False");
    appendRow("Output Device", _setup.outputDeviceName());
    appendRow("Output Channels", std::to_string(_setup.outputChannels()));
    appendRow("Output First Channel",
              std::to_string(_setup.outputFirstChannel()));
    appendRow();
    appendRow("Buffers Num.", std::to_string(_setup.buffersNum()));
    appendRow();
    appendRow("NONINTERLEAVED", _setup.checkStreamFlags(RTAUDIO_NONINTERLEAVED)
                                    ? "True"
                                    : "False");
    appendRow("MINIMIZE_LATENCY",
              _setup.checkStreamFlags(RTAUDIO_MINIMIZE_LATENCY) ? "True"
                                                                : "False");
    appendRow("HOG_DEVICE",
              _setup.checkStreamFlags(RTAUDIO_HOG_DEVICE) ? "True" : "False");
    appendRow("SCHEDULE_REALTIME",
              _setup.checkStreamFlags(RTAUDIO_SCHEDULE_REALTIME) ? "True"
                                                                 : "False");
    appendRow("ALSA_USE_DEFAULT",
              _setup.checkStreamFlags(RTAUDIO_ALSA_USE_DEFAULT) ? "True"
                                                                : "False");
    appendRow("JACK_DONT_CONNECT",
              _setup.checkStreamFlags(RTAUDIO_JACK_DONT_CONNECT) ? "True"
                                                                 : "False");

    ImGui::EndTable();
  }
}
void SetupDisplayWidget::applyStreamConfig(const RtSound::StreamSetup &setup) {
  _setup = setup;
}

void SetupDisplayWidget::appendRow(const std::string &param,
                                   const std::string &value) {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::Text("%s", param.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", value.c_str());
}
} // namespace ImSound
