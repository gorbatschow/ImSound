#include "SoundClientInfoWidget.h"
#include <RtSoundProvider.h>
#include <imgui.h>

void SoundClientInfoWidget::paint() {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.providerMutex);

  ui.holdTimeCheck.paint();
  ui.resetHold.paint();
  ui.tBufLabel.paint();
  ui.tPrcLabel.paint();

  if (ui.resetHold.handle()) {
    std::for_each(_clientTime.begin(), _clientTime.end(),
                  [](ClientTime &info) { info.time = 0; });
  }

  static const int tableCols{4};
  static const ImGuiTableColumnFlags colFlags{ImGuiTableColumnFlags_WidthFixed};

  if (ImGui::BeginTable("Client Info Table", tableCols)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type");
    ImGui::TableSetupColumn("Priority", colFlags, 50);
    ImGui::TableSetupColumn("Time us", colFlags, 50);
    ImGui::TableHeadersRow();
    for (const auto &ct : _clientTime) {
      paintRow(ct);
    }
    ImGui::EndTable();
  }
}

void SoundClientInfoWidget::applyStreamConfig(const RtSoundSetup &setup) {
  ui.tBufLabel.setValue(0);
  ui.tPrcLabel.setValue(0);

  const auto &provider{streamProvider()};
  _clientTime.clear();
  _clientTime.reserve(provider.clients().size());
  for (const auto &clientPtr : provider.clients()) {
    _clientTime.push_back({clientPtr, 0});
  }
}

void SoundClientInfoWidget::streamDataReady(const RtSoundData &data) {
  const auto &provider{streamProvider()};
  std::lock_guard(provider.providerMutex);

  ui.tBufLabel.setValue(data.framesT());
  ui.tPrcLabel.setValue(updateClientsTable());
}

long SoundClientInfoWidget::updateClientsTable() {
  const auto &provider{streamProvider()};
  long totalTime{0};
  for (auto &ct : _clientTime) {
    const auto client{ct.client.lock()};
    if (!client) {
      continue;
    }
    if (ui.holdTimeCheck()) {
      ct.time = std::max(client->streamDataReadyTime(), ct.time);
    } else {
      ct.time = client->streamDataReadyTime();
    }
    totalTime += ct.time;
  }

  return totalTime;
}

void SoundClientInfoWidget::paintRow(const ClientTime &ct) {
  const auto client{ct.client.lock()};
  if (!client) {
    assert(false);
    return;
  }

  ImGui::TableNextRow();
  // name
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(client->clientName().c_str());
  // type
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(client->clientTypeId().name());
  // priority
  ImGui::TableNextColumn();
  ImGui::Text("%d", client->priority());
  // time
  ImGui::TableNextColumn();
  ImGui::Text("%ld", ct.time);
}
