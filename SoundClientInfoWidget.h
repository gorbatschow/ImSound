#pragma once
#include <RtSoundClient.h>
#include <atomic>
#include <cstdint>
#include <imw.h>
#include <typeindex>

class SoundClientInfoWidget : public RtSoundClient {
public:
  SoundClientInfoWidget() { setClientName("SoundClientInfoWidget"); }
  ~SoundClientInfoWidget() = default;

  void paint();

  const std::type_info &clientTypeId() const override final {
    return typeid(this);
  }

private:
  struct ClientInfo {
    std::type_index typeId;
    std::uintptr_t cliendId;
    std::string name;
    int priority{};
    long time{};
    bool exists{};
    static inline const int NFIELDS{4};

    bool operator==(const std::uintptr_t &id) { return (*this).cliendId == id; }
    bool operator==(const std::weak_ptr<RtSoundClient> &ptr) {
      return (*this).cliendId == ptr.lock()->clientId();
    }
  };
  std::vector<ClientInfo> _clients;

  void applyStreamConfig(const RtSoundSetup &) override {
    _clients.clear();
    _clients.shrink_to_fit();
    updateClientsTable();
  }
  void streamDataReady(const RtSoundData &) override { updateClientsTable(); }
  void updateClientsTable();
  void paintRow(const ClientInfo &info);

  struct Ui {
    Imw::CheckBox holdTimeCheck{"Hold time"};
    Imw::Button resetHold{"Reset hold"};

    Ui() {
      resetHold.setSameLine(true);
      resetHold.setWidth(0.0f);
    }
  };
  Ui ui;
  std::atomic_bool _holdTime{};
};
