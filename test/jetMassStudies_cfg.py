import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/RunIISpring16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/premix_withHLT_80X_mcRun2_asymptotic_v14-v1/00000/042D62D1-C597-E611-8FA4-549F3525B9A0.root'
    )
)

process.jetMassStudies = cms.EDAnalyzer("JetMassStudies",
    svsrc = cms.InputTag("slimmedSecondaryVertices"),
    ak8src = cms.InputTag("slimmedJetsAK8")
)

process.p = cms.Path(process.jetMassStudies)
