#pragma once
#include <RtSoundClient.h>
#include <RtSoundIO.h>
#include <ini.h>
#include <memory>
#include <string>
#include <vector>

namespace ImSound {
struct ControlWidgetUi;

class ControlWidget : public RtSound::Client {
public:
  // Constructor
  ControlWidget(std::shared_ptr<RtSound::IO> io);

  // Destructor
  virtual ~ControlWidget() override;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }

  // Load State
  void loadState(const mINI::INIStructure &ini);
  void saveState(mINI::INIStructure &ini) const;

  // Paint
  void paint();

  inline const std::unique_ptr<ControlWidgetUi> &ui() const { return _ui; }

private:
  virtual void priorityChanged(int priority) override;
  virtual void configureStream(RtSound::StreamSetup &setup) override;

  std::shared_ptr<RtSound::IO> _soundIO;
  std::unique_ptr<ControlWidgetUi> _ui;
};

// ----------------------------------------------------------------------------

class InputControlWidget : public ControlWidget
{
public:
  // Constructor
  InputControlWidget(const std::shared_ptr<RtSound::IO> &io);

  // Destructor
  virtual ~InputControlWidget() override;

  // Client Type Id
  virtual const std::type_info &clientTypeId() const override {
    return typeid(this);
  }
};

} // namespace ImSound
