import FWCore.ParameterSet.Config as cms
# ---------- Add assigned jet-track association

from RecoJets.JetAssociationProducers.trackExtrapolator_cfi import *
trackExtrapolator.trackSrc = cms.InputTag("produceTracks:tracksFromPF")
trackExtrapolator.trackQuality = cms.string('highPurity')

from RecoJets.JetAssociationProducers.ak4JTA_cff import *
ak4JetTracksAssociatorAtVertexJPTPAT = ak4JetTracksAssociatorAtVertex.clone()
ak4JetTracksAssociatorAtVertexJPTPAT.useAssigned = cms.bool(True)
ak4JetTracksAssociatorAtVertexJPTPAT.pvSrc = cms.InputTag("offlineSlimmedPrimaryVertices")
ak4JetTracksAssociatorAtVertexJPTPAT.jets = cms.InputTag("slimmedCaloJets")
ak4JetTracksAssociatorAtVertexJPTPAT.tracks = cms.InputTag("produceTracks:tracksFromPF")
ak4JetTracksAssociatorAtCaloFaceJPTPAT = ak4JetTracksAssociatorAtCaloFace.clone()
ak4JetTracksAssociatorAtCaloFaceJPTPAT.jets = cms.InputTag("slimmedCaloJets")
ak4JetTracksAssociatorAtCaloFaceJPTPAT.tracks = cms.InputTag("produceTracks:tracksFromPF")
ak4JetExtenderJPTPAT = ak4JetExtender.clone()
ak4JetExtenderJPTPAT.jets = cms.InputTag("slimmedCaloJets")
ak4JetExtenderJPTPAT.jet2TracksAtCALO = cms.InputTag("ak4JetTracksAssociatorAtCaloFaceJPTPAT")
ak4JetExtenderJPTPAT.jet2TracksAtVX = cms.InputTag("ak4JetTracksAssociatorAtVertexJPTPAT")

# ---------- Supported Modules

produceTracks = cms.EDProducer("TrackFromPFProducer",
                                PFCandidates = cms.InputTag('packedPFCandidates'),
                                PFCandidatesLostTracks = cms.InputTag('lostTracks')
)

JetPlusTrackAddonSeedRecoPAT = cms.EDProducer(
    "JetPlusTrackAddonSeedProducer",
    srcCaloJets = cms.InputTag("slimmedCaloJets"),
    srcTrackJets = cms.InputTag("ak4TrackJets"),
    srcPVs = cms.InputTag('offlineSlimmedPrimaryVertices'),
    ptCUT = cms.double(15.),
    dRcone = cms.double(0.4),
    PFCandidates = cms.InputTag('packedPFCandidates'),
    towerMaker = cms.InputTag('towerMaker'),
    UsePAT = cms.bool(True)
)

from CommonTools.RecoAlgos.TrackWithVertexRefSelector_cfi import *
trackWithVertexRefSelector.vertexTag = cms.InputTag('offlineSlimmedPrimaryVertices')
trackWithVertexRefSelector.src = cms.InputTag('produceTracks:tracksFromPF')
from RecoJets.JetProducers.TracksForJets_cff import *
from RecoJets.Configuration.RecoTrackJets_cff import *
ak4TrackJets.srcPVs = cms.InputTag('offlineSlimmedPrimaryVertices')
ak4TrackJets.UseOnlyOnePV = cms.bool(True)

# ---------- Module definition

from RecoJets.JetPlusTracks.JetPlusTrackCorrections_cfi import *

JetPlusTrackZSPCorJetAntiKt4PAT = cms.EDProducer(
    "JetPlusTrackProducer",
    cms.PSet(JPTZSPCorrectorAntiKt4),
    src = cms.InputTag("slimmedCaloJets"),
    srcTrackJets = cms.InputTag("ak4TrackJets"), 
    srcAddCaloJets = cms.InputTag('JetPlusTrackAddonSeedRecoPAT:ak4CaloJetsJPTSeed'),
    extrapolations = cms.InputTag("trackExtrapolator"),
    tagName = cms.vstring('ZSP_CMSSW390_Akt_05_PU0'),
    tagNameOffset = cms.vstring(),
    PU = cms.int32(-1),
    FixedPU = cms.int32(0),
    UseZSP = cms.bool(False),
    srcPVs = cms.InputTag('offlineSlimmedPrimaryVertices'),    
    alias = cms.untracked.string('JetPlusTrackZSPCorJetAntiKt4'),
    ptCUT = cms.double(15.),
    dRcone = cms.double(0.4)
    )

JetPlusTrackZSPCorJetAntiKt4PAT.JetTracksAssociationAtVertex = cms.InputTag("ak4JetTracksAssociatorAtVertexJPTPAT")
JetPlusTrackZSPCorJetAntiKt4PAT.JetTracksAssociationAtCaloFace = cms.InputTag("ak4JetTracksAssociatorAtCaloFaceJPTPAT")
JetPlusTrackZSPCorJetAntiKt4PAT.Muons = cms.InputTag("slimmedMuons")
JetPlusTrackZSPCorJetAntiKt4PAT.Electrons = cms.InputTag("slimmedElectrons")
JetPlusTrackZSPCorJetAntiKt4PAT.JetSplitMerge = cms.int32(2)
JetPlusTrackZSPCorJetAntiKt4PAT.UseReco = cms.bool(False)
### ---------- Sequences

# Anti-Kt

JetPlusTrackCorrectionsAntiKt4TaskPAT = cms.Task(
    produceTracks,
    trackWithVertexRefSelector,
    trackRefsForJets,
    ak4TrackJets,
    trackExtrapolator,
    JetPlusTrackAddonSeedRecoPAT,
    ak4JetTracksAssociatorAtVertexJPTPAT,
    ak4JetTracksAssociatorAtCaloFaceJPTPAT,
    ak4JetExtenderJPTPAT,
    JetPlusTrackZSPCorJetAntiKt4PAT
    )

JetPlusTrackCorrectionsAntiKt4PAT = cms.Sequence(JetPlusTrackCorrectionsAntiKt4TaskPAT)

# For backward-compatiblity (but to be deprecated!)

JetPlusTrackCorrectionsPAT = cms.Sequence(JetPlusTrackCorrectionsAntiKt4PAT)
