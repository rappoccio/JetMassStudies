import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_9_2_7/RelValTTbar_13/MINIAODSIM/92X_upgrade2017_realistic_v7-v2/00000/743B5BB7-BB70-E711-A6C9-0CC47A4D75F0.root',
'/store/relval/CMSSW_9_2_7/RelValTTbar_13/MINIAODSIM/92X_upgrade2017_realistic_v7-v2/00000/BE06BD72-BB70-E711-9B7D-0CC47A78A414.root'

    )
)

process.jetMassStudies = cms.EDAnalyzer("JetMassStudies",
    svsrc = cms.InputTag("generalV0Candidates"),
    ak8src = cms.InputTag("slimmedJetsAK8")
)

process.p = cms.Path(process.jetMassStudies)
