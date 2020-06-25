#ifndef TrackProducer_h
#define TrackProducer_h

/** \class TrackProducer
 *  Produce Tracks from TrackCandidates
 *
*/
//
// Original Author:  Olga Kodolova,40 R-A12,+41227671273,
//         Created:  Fri Feb 19 10:14:02 CET 2010
//
//
//

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

class TrackFromPFProducer : public edm::stream::EDProducer<> {
public:
  /// Constructor
  explicit TrackFromPFProducer(const edm::ParameterSet& iConfig);

  /// Implementation of produce method
  void produce(edm::Event&, const edm::EventSetup&) override;

private:
  edm::EDGetTokenT<std::vector<pat::PackedCandidate> > tokenPFCandidates_;
  edm::EDGetTokenT<std::vector<pat::PackedCandidate> > tokenPFCandidatesLostTracks_;
};

#endif
