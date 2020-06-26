/** \class TrackProducer
 *  Produce Tracks from TrackCandidates
 *  
*/
//
// Original Author:  Olga Kodolova,40 R-A12,+41227671273,
//         Created:  Fri Feb 19 10:14:02 CET 2010
//
//

// system include files
#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/isFinite.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

class TrackFromPackedCandidateProducer : public edm::stream::EDProducer<> {
public:
  /// Constructor
  explicit TrackFromPackedCandidateProducer(const edm::ParameterSet& iConfig);

  /// fill descriptions
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  /// Implementation of produce method
  void produce(edm::Event&, const edm::EventSetup&) override;

private:
  const edm::EDGetTokenT<std::vector<pat::PackedCandidate> > tokenPFCandidates_;
  const edm::EDGetTokenT<std::vector<pat::PackedCandidate> > tokenPFCandidatesLostTracks_;
};

TrackFromPackedCandidateProducer::TrackFromPackedCandidateProducer(const edm::ParameterSet& iConfig)
    : tokenPFCandidates_(consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("PFCandidates"))),
      tokenPFCandidatesLostTracks_(
          consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("PFCandidatesLostTracks"))) {
  produces<reco::TrackCollection>();
}

void TrackFromPackedCandidateProducer::fillDescriptions(edm::ConfigurationDescriptions& iDescriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("PFCandidates");
  desc.add<edm::InputTag>("PFCandidatesLostTracks");
  iDescriptions.addDefault(desc);
}

void TrackFromPackedCandidateProducer::produce(edm::Event& theEvent, const edm::EventSetup& setup) {
  //
  // create empty output collections
  //
  auto outputTColl = std::make_unique<reco::TrackCollection>();
  auto const pfCandidates = theEvent.get(tokenPFCandidates_);
  outputTColl->reserve(pfCandidates.size());

  for (auto const& pf : pfCandidates) {
    if (pf.hasTrackDetails()) {
      const reco::Track& mytrack = pf.pseudoTrack();
      using namespace edm;
      if (isNotFinite(mytrack.pt()) || isNotFinite(mytrack.eta()) || isNotFinite(mytrack.phi()))
        continue;
      outputTColl->push_back(mytrack);
    }
  }

  //put everything in the event
  theEvent.put(std::move(outputTColl));
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TrackFromPackedCandidateProducer);
