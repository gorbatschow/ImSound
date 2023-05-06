#include "ImSoundClientInfoWidget.h"
#include <imgui.h>
#include <imwrap.h>
#include <mutex>

namespace ImSound {

struct ClientInfoWidget::ClientTime {
  std::weak_ptr<RtSound::Client> client;
  long time{};

  ClientTime(const std::shared_ptr<RtSound::Client> &ptr) : client{ptr} {}
};

struct ClientInfoWidget::Ui {
  ImWrap::CheckBox holdTimeCheck{"Hold time"};
  ImWrap::Button resetHold{"Reset"};
  ImWrap::ValueLabel<double> tRunLabel{"Trun=%.1f"};
  ImWrap::ValueLabel<int> tBufLabel{"Tbuf=%ld"};
  ImWrap::ValueLabel<int> tPrcLabel{"Tprc=%ld"};

  Ui() {
    resetHold.setSameLine(true);
    // resetHold.setWidth(0.0f);
    tRunLabel.setSameLine(true);
    tBufLabel.setSameLine(true);
    tPrcLabel.setSameLine(true);
  }
};

ClientInfoWidget::ClientInfoWidget() : _ui{std::make_unique<Ui>()} {
  setClientName("ImSound::ClientInfoWidget");
}

ClientInfoWidget::~ClientInfoWidget() = default;

void ClientInfoWidget::loadState(const mINI::INIStructure &ini) {
  _ui->holdTimeCheck.loadState(ini);
}

void ClientInfoWidget::saveState(mINI::INIStructure &ini) const {
  _ui->holdTimeCheck.saveState(ini);
}

void ClientInfoWidget::loadWidgetState() {
  _ui->holdTimeCheck.loadStateFromFile();
}

void ClientInfoWidget::paint() {
  std::scoped_lock lock{clientMutex};

  _ui->holdTimeCheck.paint();
  _ui->resetHold.paint();
  _ui->tRunLabel.paint();
  _ui->tBufLabel.paint();
  _ui->tPrcLabel.paint();

  if (_ui->resetHold.handle()) {
    std::for_each(std::begin(_clientTime), std::end(_clientTime),
                  [](const auto &info) { info->time = 0; });
    _ui->tPrcLabel.setValue(0);
  }

  static const int tableCols{4};
  static const ImGuiTableColumnFlags colFlags{ImGuiTableColumnFlags_WidthFixed};

  if (ImGui::BeginTable("Client Info Table", tableCols)) {
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Type ID");
    ImGui::TableSetupColumn("Priority", colFlags, 50);
    ImGui::TableSetupColumn("Time [us]", colFlags, 80);
    ImGui::TableHeadersRow();
    for (const auto &ct : _clientTime) {
      paintRow(*ct);
    }
    ImGui::EndTable();
  }
}

void ClientInfoWidget::updateSoundClients(
    const std::vector<std::shared_ptr<Client>> &clients) {
  _ui->tRunLabel.setValue({});
  _ui->tBufLabel.setValue({});
  _ui->tPrcLabel.setValue({});
  _clientTime.clear();
  _clientTime.reserve(clients.size());
  for (const auto &client : clients) {
    _clientTime.push_back(std::make_unique<ClientTime>(client));
  }
}

void ClientInfoWidget::streamDataReady(const RtSound::StreamData &data) {
  std::scoped_lock lock(clientMutex);
  _ui->tRunLabel.setValue(data.streamTime());
  _ui->tBufLabel.setValue(data.framesT_us());
  _ui->tPrcLabel.setValue(updateClientsTable());
}

long ClientInfoWidget::updateClientsTable() {
  long totalTime{};
  for (auto &ct : _clientTime) {
    const auto client{ct->client.lock()};
    if (!client) {
      continue;
    }
    if (_ui->holdTimeCheck()) {
      ct->time = std::max(client->streamDataReadyTime(), ct->time);
    } else {
      ct->time = client->streamDataReadyTime();
    }
    totalTime += ct->time;
  }
  return totalTime;
}

void ClientInfoWidget::paintRow(const ClientTime &ct) {
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
  ImGui::Text("%d", client->clientPriority());
  // time
  ImGui::TableNextColumn();
  ImGui::Text("%ld", ct.time);
}
} // namespace ImSound
