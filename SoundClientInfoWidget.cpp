#include "SoundClientInfoWidget.h"
#include <RtSoundProvider.h>
#include <imgui.h>

void SoundClientInfoWidget::paint() {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.mutex);
  if (ImGui::BeginTable("Client Info Table", ClientInfo::NFIELDS)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Priority");
    ImGui::TableSetupColumn("Time");
    ImGui::TableHeadersRow();
    for (const auto &info : _clientInfo) {
      appendRow(info);
    }
    ImGui::EndTable();
  }
}

void SoundClientInfoWidget::streamDataReady(const RtSoundData &) {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.mutex);
  _clientInfo.clear();
  _clientInfo.reserve(provider.clients().size());
  for (const auto &client : provider.clients()) {
    const auto &ptr{*client.lock()};
    _clientInfo.push_back({ptr.clientTypeId(), ptr.clientName(), ptr.priority(),
                           ptr.streamDataReadyTime()});
  }
}

void SoundClientInfoWidget::appendRow(const ClientInfo &info) {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(info.name.c_str());
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(info.typeId.name());
  ImGui::TableNextColumn();
  ImGui::Text("%d", info.priority);
  ImGui::TableNextColumn();
  ImGui::Text("%ld", info.time);
}
