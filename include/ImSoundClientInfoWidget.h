#pragma once
#include "ini.h"
#include <RtSoundClient.h>
#include <cstdint>
#include <typeindex>

namespace ImSound {
class ClientInfoWidget : public RtSound::Client
{
public:
  // Constructor
  ClientInfoWidget();

  // Destructor
  ~ClientInfoWidget();

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;
  void loadWidgetState();

  // Paint
  void paint();

private:
  struct ClientTime;
  std::vector<std::unique_ptr<ClientTime> > _clientTime;

  struct Ui;
  std::unique_ptr<Ui> _ui;

  virtual void updateSoundClients(
      const std::vector<std::shared_ptr<Client> > &clients) override;
  virtual void streamDataReady(const RtSound::StreamData &data) override;
  long updateClientsTable();
  void paintRow(const ClientTime &client);
};
} // namespace ImSound
