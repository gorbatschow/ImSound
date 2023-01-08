#include "SoundClientInfoWidget.h"
#include <RtSoundProvider.h>
#include <imgui.h>

void SoundClientInfoWidget::paint() {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.mutex);

  ui.holdTimeCheck.paint();
  ui.resetHold.paint();

  if (ui.holdTimeCheck.handle()) {
    _holdTime.exchange(ui.holdTimeCheck());
  }

  if (ui.resetHold.handle()) {
    std::for_each(_clients.begin(), _clients.end(),
                  [](ClientInfo &info) { info.time = 0; });
  }

  if (ImGui::BeginTable("Client Info Table", ClientInfo::NFIELDS)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Priority");
    ImGui::TableSetupColumn("Time us");
    ImGui::TableHeadersRow();
    for (const auto &info : _clients) {
      paintRow(info);
    }
    ImGui::EndTable();
  }
}

void SoundClientInfoWidget::updateClientsTable() {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.mutex);

  std::for_each(_clients.begin(), _clients.end(),
                [](ClientInfo &info) { info.exists = false; });
  for (const auto &clientPtr : provider.clients()) {
    const auto it{std::find(_clients.begin(), _clients.end(), clientPtr)};
    const auto &client{*clientPtr.lock()};
    if (it == _clients.end()) {
      _clients.push_back({client.clientTypeId(), client.clientId(),
                          client.clientName(), client.priority(),
                          client.streamDataReadyTime(), true});
    } else {
      auto &info = (*it);
      info.exists = true;
      if (_holdTime.load()) {
        info.time = std::max(client.streamDataReadyTime(), info.time);
      } else {
        info.time = client.streamDataReadyTime();
      }
    }
  }
  std::remove_if(_clients.begin(), _clients.end(),
                 [](const ClientInfo &info) { return info.exists == false; });
}

void SoundClientInfoWidget::paintRow(const ClientInfo &info) {
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
