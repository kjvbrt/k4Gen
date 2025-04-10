
#ifndef GENERATION_HEPMCPILEMERGETOOL_H
#define GENERATION_HEPMCPILEMERGETOOL_H

#include "Generation/IHepMCMergeTool.h"
#include "k4FWCore/DataHandle.h"

#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/RndmGenerators.h"

class HepMCSimpleMerge final : public AlgTool, virtual public IHepMCMergeTool {
public:
  HepMCSimpleMerge(const std::string& type, const std::string& name, const IInterface* parent);

  virtual ~HepMCSimpleMerge() final;

  virtual StatusCode initialize() final;

  virtual StatusCode finalize() final;

  /** Merge the events in the container into the signalEvent
   * For events in the vector, new vertices in signalEvent are created to which the final-state particles are attached.
   *  @param[in/out] signalEvent is the signal event that will be enriched with puEvents from eventVector
   *  @param[in] eventVector is the vector of pile-up GenEvents
   */
  virtual StatusCode merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) final;
};

#endif // GENERATION_HEPMCPILEMERGETOOL_H
