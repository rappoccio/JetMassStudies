import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService = cms.Service("TFileService", fileName = cms.string("histo.root") )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/mc/RunIISummer16DR80Premix/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/60000/048DCC32-53BB-E611-A3AB-B083FED4263D.root'
#'/store/mc/RunIISummer16DR80Premix/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/00047D71-E7BA-E611-A259-001E4F1B8E39.root'
#'/store/relval/CMSSW_9_2_7/RelValTTbar_13/MINIAODSIM/92X_upgrade2017_realistic_v7-v2/00000/743B5BB7-BB70-E711-A6C9-0CC47A4D75F0.root',
#'/store/relval/CMSSW_9_2_7/RelValTTbar_13/MINIAODSIM/92X_upgrade2017_realistic_v7-v2/00000/BE06BD72-BB70-E711-9B7D-0CC47A78A414.root'
    )
)

process.jetMassStudies = cms.EDAnalyzer("JetMassStudies",
    svsrc = cms.InputTag("generalV0Candidates","Kshort"),
    ak8src = cms.InputTag("ak8PFJetsCHS")
)

process.p = cms.Path(process.jetMassStudies)
