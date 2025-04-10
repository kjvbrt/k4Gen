#include "HepMCSimpleMerge.h"

#include "GaudiKernel/IEventProcessor.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

DECLARE_COMPONENT(HepMCSimpleMerge)

HepMCSimpleMerge::HepMCSimpleMerge(const std::string& type, const std::string& name, const IInterface* parent)
    : AlgTool(type, name, parent) {
  declareInterface<IHepMCMergeTool>(this);
}

HepMCSimpleMerge::~HepMCSimpleMerge() {}

StatusCode HepMCSimpleMerge::initialize() {
  StatusCode sc = AlgTool::initialize();
  if (!sc.isSuccess())
    return sc;
  return sc;
}

StatusCode HepMCSimpleMerge::merge(HepMC3::GenEvent& signalEvent, const std::vector<HepMC3::GenEvent>& eventVector) {
  // iterate over vertices and add them to signalEvent
  for (auto it = eventVector.cbegin(), end = eventVector.cend(); it != end; ++it) {
    std::unordered_map<std::shared_ptr<const HepMC3::GenVertex>, std::shared_ptr<HepMC3::GenVertex>>
        inputToMergedVertexMap;
    for (auto& v : it->vertices()) {
      auto newVertex = std::make_shared<HepMC3::GenVertex>(v->position());
      inputToMergedVertexMap[v] = newVertex;
    }
    for (auto& p : it->particles()) {
      // simple check if final-state particle:
      // has no end vertex and correct status code meaning no further decays
      if (!p->end_vertex() && p->status() == 1) {
        // ownership of the particle  (newParticle) is then given to the vertex (newVertex)
        auto newParticle = std::make_shared<HepMC3::GenParticle>(*p);
        // each pile up particle is associated to a new production vertex
        // the position information is preserved
        // ownership of the vertex (newVertex) is given to the event (newEvent)
        auto newVertex = inputToMergedVertexMap[p->production_vertex()];
        newVertex->add_particle_out(newParticle);
        signalEvent.add_vertex(newVertex);
      }
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode HepMCSimpleMerge::finalize() { return AlgTool::finalize(); }
