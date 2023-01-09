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
  struct ClientTime {
    std::weak_ptr<RtSoundClient> client;
    long time{};
  };
  std::vector<ClientTime> _clientTime;

  void applyStreamConfig(const RtSoundSetup &setup) override;
  void streamDataReady(const RtSoundData &data) override;
  long updateClientsTable();
  void paintRow(const ClientTime &client);

  struct Ui {
    Imw::CheckBox holdTimeCheck{"Hold time"};
    Imw::Button resetHold{"Reset"};
    Imw::ValueLabel<int> tBufLabel{"Tbuf=%ld"};
    Imw::ValueLabel<int> tPrcLabel{"Tprc=%ld"};

    Ui() {
      resetHold.setSameLine(true);
      // resetHold.setWidth(0.0f);
      tBufLabel.setSameLine(true);
      tPrcLabel.setSameLine(true);
    }
  };
  Ui ui;
};
