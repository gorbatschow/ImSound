#pragma once
#include <RtSoundClient.h>
#include <atomic>
#include <cstdint>
#include <imw.h>
#include <typeindex>

namespace ImSound {
class ClientInfoWidget : public RtSound::Client
{
public:
  // Constructor
  ClientInfoWidget() { setClientName("SoundClientInfoWidget"); }

  // Destructor
  ~ClientInfoWidget() = default;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  void loadWidgetState();

  // Paint
  void paint();

private:
  struct ClientTime {
    std::weak_ptr<RtSound::Client> client;
    long time{};
  };
  std::vector<ClientTime> _clientTime;

  void applyStreamConfig(const RtSound::StreamSetup &setup) override;
  void streamDataReady(const RtSound::StreamData &data) override;
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
} // namespace ImSound
