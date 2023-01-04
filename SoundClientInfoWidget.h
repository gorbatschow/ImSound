#pragma once
#include <RtSoundClient.h>

class SoundClientInfoWidget : public RtSoundClient {
public:
  SoundClientInfoWidget() = default;
  ~SoundClientInfoWidget() = default;

  void paint();

  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

private:
  struct ClientInfo {
    const std::type_info &typeId;
    std::string name;
    int priority{};
    long time{};
    bool exists{};
    static inline const int NFIELDS{4};
  };
  std::vector<ClientInfo> _clientInfo;

  void streamDataReady(const RtSoundData &) override;
  void appendRow(const ClientInfo &info);
};
