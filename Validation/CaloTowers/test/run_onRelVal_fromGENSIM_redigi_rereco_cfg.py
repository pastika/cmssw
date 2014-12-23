import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

### RANDOM setting (change last digit(s) to make runs different !)
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

#process.load("Configuration.StandardSequences.Simulation_cff")
process.load("SimCalorimetry.Configuration.hcalDigiSequence_cff")
process.load("SimGeneral.MixingModule.mixNoPU_cfi")
#process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
#process.load("RecoJets.JetAssociationProducers.trackExtrapolator_cfi")
process.load("RecoLocalCalo.Configuration.hcalGlobalReco_cff")
process.load("RecoMET.METProducers.hcalnoiseinfoproducer_cfi")
process.load("RecoJets.Configuration.CaloTowersRec_cff")
process.load("RecoLocalCalo.EcalRecAlgos.EcalSeverityLevelESProducer_cfi")
#process.load("Configuration.StandardSequences.Reconstruction_cff")
#process.load('Configuration.StandardSequences.DigiToRaw_cff')
#process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load("EventFilter.HcalRawToDigi.HcalDigiToRaw_cfi")
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("EventFilter.RawDataCollector.rawDataCollector_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.autoCond import autoCond
#process.GlobalTag.globaltag = autoCond['startup']
process.GlobalTag.globaltag = cms.string('MCRUN2_73_V5::All')

#process.load("IOMC.EventVertexGenerators.VtxSmearedGauss_cfi")
#process.load("Configuration.StandardSequences.GeometryECALHCAL_cff")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#process.g4SimHits.UseMagneticField = False

#process.load("DQMServices.Core.DQM_cfg")
#process.DQM.collectorHost = ''

process.load("DQMServices.Core.DQMStore_cfi")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) 
)
# Input source
process.source = cms.Source("PoolSource",
#    noEventSort = cms.untracked.bool(True),
#    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-RECO/MCRUN2_73_V5-v1/00000/62353C09-5D76-E411-AEC7-02163E010EC9.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-RECO/MCRUN2_73_V5-v1/00000/C49EE3D3-A676-E411-9AEE-02163E010DFA.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-RECO/MCRUN2_73_V5-v1/00000/D071BD3E-2D77-E411-9B3C-02163E00EA43.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-RECO/MCRUN2_73_V5-v1/00000/E68DDD56-2777-E411-9C78-02163E00F541.root'
    ) ,
    secondaryFileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/10D636E4-2E76-E411-AF93-02163E00C8A7.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/34B37E29-3276-E411-BD88-02163E00F48D.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/441665DD-2E76-E411-8D1E-02163E010F12.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/4A9F73BD-2E76-E411-BD31-02163E00EA79.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/74C5D9DE-2E76-E411-BD57-02163E010BFA.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/B0D01173-2F76-E411-85CC-0025904B26CA.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/D806C928-2D76-E411-A25D-02163E010293.root',
        '/store/relval/CMSSW_7_3_0_pre3/RelValQCD_Pt_3000_3500_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_73_V5-v1/00000/DC05E9B6-3076-E411-AEEA-02163E00E9DE.root'
    ),
#    inputCommands = cms.untracked.vstring("keep *",
#        'drop *_simHcalUnsuppressedDigis_*_*',
#        'drop *_simHcalDigis_*_*',
#        'drop *_hbhereco_*_*',
#        'drop *_hfreco_*_*',
#        'drop *_horeco_*_*'
#        )
   inputCommands = cms.untracked.vstring('keep *',
                                         'drop *_MEtoEDMConverter_*_*',
                                         'drop HBHEDataFramesSorted_*__*',
                                         'drop HODataFramesSorted_*__*',
                                         'drop HFDataFramesSorted_*__*',
                                         'drop *_MEtoEDMConverter_*_*'
                                         )
    ) 

process.FEVT = cms.OutputModule("PoolOutputModule",
     outputCommands = cms.untracked.vstring('drop *', 'keep *_MEtoEDMConverter_*_*'),
     splitLevel = cms.untracked.int32(0),
     fileName = cms.untracked.string("output.root")
)

process.HcalSimHitsAnalyser = cms.EDAnalyzer("HcalSimHitsValidation",
    outputFile = cms.untracked.string('HcalSimHitsValidation.root')
)   

process.hcalDigiAnalyzer = cms.EDAnalyzer("HcalDigisValidation",
    outputFile		      = cms.untracked.string('HcalDigisValidationRelVal.root'),
    digiLabel		      = cms.InputTag("hcalDigis"),
    zside		      = cms.untracked.string('*'),
    mode		      = cms.untracked.string('multi'),

    hcalselector	      = cms.untracked.string('all'),
    mc			      = cms.untracked.string('yes') # 'yes' for MC
)   

process.hcalRecoAnalyzer = cms.EDAnalyzer("HcalRecHitsValidation",
    outputFile                = cms.untracked.string('HcalRecHitValidationRelVal.root'),
    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbhereco"),
    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfreco"),
    HORecHitCollectionLabel   = cms.untracked.InputTag("horeco"),
    eventype                  = cms.untracked.string('single'),
    ecalselector              = cms.untracked.string('yes'),
    hcalselector              = cms.untracked.string('all'),
    mc                        = cms.untracked.string('yes')  # default !
)

process.hcalTowerAnalyzer = cms.EDAnalyzer("CaloTowersValidation",
    outputFile               = cms.untracked.string('CaloTowersValidationRelVal.root'),
    CaloTowerCollectionLabel = cms.untracked.InputTag('towerMaker'),
    hcalselector             = cms.untracked.string('all'),
    mc                       = cms.untracked.string('yes')  # default!
)


#--- EventFilter/HcalRawToDigi/python/HcalDigiToRaw_cfi.py
#process.hcalRawData = cms.EDProducer("HcalDigiToRaw",
#    HBHE = cms.untracked.InputTag("simHcalDigis"),
#    HF = cms.untracked.InputTag("simHcalDigis"),
#    HO = cms.untracked.InputTag("simHcalDigis")
#)
#--- EventFilter/HcalRawToDigi/python/HcalRawToDigi_cfi.py
process.hcalDigis.UnpackZDC =  cms.untracked.bool(False)

#--- DigiToRaw_cff.py
#process.ecalPacker.Label = 'simEcalDigis'
#process.ecalPacker.InstanceEB = 'ebDigis'
#process.ecalPacker.InstanceEE = 'eeDigis'
#process.ecalPacker.labelEBSRFlags = "simEcalDigis:ebSrFlags"
#process.ecalPacker.labelEESRFlags = "simEcalDigis:eeSrFlags"

#--- to force RAW->Digi 
#process.ecalDigis.InputLabel = 'rawDataCollector'
process.hcalDigis.InputLabel = 'rawDataCollector'
#process.ecalPreshowerDigis.sourceTag = 'rawDataCollector'

#--- replace hbhereco with hbheprereco
delattr(process,"hbhereco")
process.hbhereco = process.hbheprereco.clone()
process.hcalLocalRecoSequence = cms.Sequence(process.hbhereco+process.hfreco+process.horeco)


#---  Mixing:  ECAL and HCAL
process.mix.digitizers = cms.PSet(
#      ecal = cms.PSet(
#        process.ecalDigitizer
#      ),
      hcal = cms.PSet(
        process.hcalDigitizer
      )
)

#from SimGeneral.MixingModule.mixObjects_cfi import * 
#process.mix.mixObjects = cms.PSet(
#        mixCH = cms.PSet(
#            mixCaloHits
#        ),
#        mixHepMC = cms.PSet(
#           mixHepMCProducts
#        )
#)
#process.mix.mixObjects.mixCH = cms.PSet(
#    input = cms.VInputTag(
#                          cms.InputTag("g4SimHits","EcalHitsEB"), 
#                          cms.InputTag("g4SimHits","EcalHitsEE"), 
#                          cms.InputTag("g4SimHits","EcalHitsES"),
#                          cms.InputTag("g4SimHits","HcalHits")),
#    type = cms.string('PCaloHit'),
#    subdets = cms.vstring( 
#        'EcalHitsEB', 
#        'EcalHitsEE', 
#        'EcalHitsES', 
#        'HcalHits' 
#        ),
#    crossingFrames = cms.untracked.vstring(
#        'EcalHitsEB', 
#        'EcalHitsEE', 
#        'EcalHitsES', 
#        'HcalHits')
#)
#process.mixCaloHits = cms.PSet(
#    input = cms.VInputTag(
#                          cms.InputTag("g4SimHits","EcalHitsEB"),
#                          cms.InputTag("g4SimHits","EcalHitsEE"), 
#                          cms.InputTag("g4SimHits","EcalHitsES"), 
#                          cms.InputTag("g4SimHits","HcalHits")),
#    type = cms.string('PCaloHit'),
#    subdets = cms.vstring(
#        'EcalHitsEB', 
#        'EcalHitsEE', 
#        'EcalHitsES', 
#        'HcalHits'
#        ),
#    crossingFrames = cms.untracked.vstring(
#        'EcalHitsEB', 
#        'EcalHitsEE', 
#        'EcalHitsES', 
#        'HcalHits')
#)
process.simCastorDigis = cms.EDAlias()
process.simSiPixelDigis = cms.EDAlias()
process.simSiStripDigis = cms.EDAlias()
process.simEcalUnsuppressedDigis = cms.EDAlias()

#--- noPU ECAL addition in 730pre2
#process.ecalMultiFitUncalibRecHit.activeBXs = cms.vint32(-4,-2,0,2,4)
#process.ecalMultiFitUncalibRecHit.useLumiInfoRunHeader = False

#process.rawDataCollector.RawCollectionList = cms.VInputTag( #cms.InputTag('SiStripDigiToRaw'),
                                                            #cms.InputTag('castorRawData'),
                                                            #cms.InputTag('cscpacker', 'CSCRawData'),
                                                            #cms.InputTag('csctfpacker', 'CSCTFRawData'),
                                                            #cms.InputTag('dtpacker'),
                                                            #cms.InputTag('dttfpacker'),
                                                            #cms.InputTag('ecalPacker'),
                                                            #cms.InputTag('esDigiToRaw'),
                                                            #cms.InputTag('gctDigiToRaw'),
                                                            #cms.InputTag('hcalRawData'))
                                                            #cms.InputTag('l1GtEvmPack'),
                                                            #cms.InputTag('l1GtPack'),
                                                            #cms.InputTag('rpcpacker'),
                                                            #cms.InputTag('siPixelRawData')

process.load('Configuration/StandardSequences/EDMtoMEAtRunEnd_cff')
process.dqmSaver.referenceHandling = cms.untracked.string('all')

cmssw_version = os.environ.get('CMSSW_VERSION','CMSSW_X_Y_Z')
Workflow = '/HcalValidation/'+'Harvesting/'+str(cmssw_version)
process.dqmSaver.workflow = Workflow

#---------- PATH
#process.g4SimHits.Generator.HepMCProductLabel = 'generator'
#process.p = cms.Path(
## process.VtxSmeared * process.g4SimHits * 
# process.mix *
## process.ecalDigiSequence * 
# process.hcalDigiSequence *
## process.ecalPacker *
## process.esDigiToRaw *
# process.hcalRawData *
# process.rawDataCollector *
## process.ecalDigis *
## process.ecalPreshowerDigis *
# process.hcalDigis *
# process.hcalLocalRecoSequence *
## process.calolocalreco *
# process.caloTowersRec *
# process.hcalnoise *
## process.HcalSimHitsAnalyser *
# process.hcalDigiAnalyzer *
# process.hcalTowerAnalyzer *
# process.hcalRecoAnalyzer *
# #process.MEtoEDMConverter
# process.dqmSaver
#)

# process.ecalSeverityLevel *

process.p = cms.Path(
 process.mix *
 process.hcalDigiSequence *
 process.hcalRawData *
 process.rawDataCollector *
 process.hcalDigis *
 process.hcalLocalRecoSequence *
# process.trackExtrapolator *
# process.hcalGlobalRecoSequence * 
 process.caloTowersRec *
 process.hcalnoise *
 process.hcalDigiAnalyzer *
 process.hcalTowerAnalyzer *
 process.hcalRecoAnalyzer *
 process.dqmSaver
)

#process.outpath = cms.EndPath(process.FEVT)

# Customization

from SLHCUpgradeSimulations.Configuration.postLS1Customs import customise_Digi
customise_Digi(process)
