import os
import FWCore.ParameterSet.Config as cms

process = cms.Process("hcalval")
#process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
#process.load("Configuration.Geometry.GeometryExtended2023HGCalV4Reco_cff")
process.load("Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

# I added this hoping it will resolve hcaldigis
#process.load('Configuration/StandardSequences/DigiToRaw_cff')
#process.load('Configuration/StandardSequences/RawToDigi_cff')


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'DES19_62_V7::All', '')

#process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("DQMServices.Core.DQM_cfg")
process.DQM.collectorHost = ''

process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

#######################################################################
# TWO-file approach, as both RAW  (for HCAL re-reco)    and
#                               RECO (for unchanged ECAL)  are required 
#######################################################################
process.source = cms.Source("PoolSource",
noEventSort = cms.untracked.bool(True),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),   
    #--- full set of GEN-SIM-RECO RelVal files ----------------------------
#    fileNames = cms.untracked.vstring('file:QCD_30_35_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO.root'
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/9CBA2121-B25F-E411-8509-0025905A48C0.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/AA20018C-B25F-E411-8663-0025905A60A8.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/BAC0743E-BE5F-E411-AABE-0025905A612C.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/C8409C8D-BE5F-E411-874E-0025905A6080.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/F609D51A-B65F-E411-8377-002354EF3BE2.root',
       '/store/relval/CMSSW_6_2_0_SLHC20/RelValQCD_Pt_80_120_14TeV/GEN-SIM-RECO/DES23_62_V1_UPGHGCalV5-v1/00000/FE109A89-BE5F-E411-BABE-0025905B85EE.root'
    ),
    #--- full set of GEN-SIM-DIGI-RAW(-HLTDEBUG) RelVal files -------------
    secondaryFileNames = cms.untracked.vstring(
     ),  
    inputCommands = cms.untracked.vstring('keep *', 'drop *_MEtoEDMConverter_*_*')
)


process.load("DQMServices.Components.MEtoEDMConverter_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1500)
)

process.FEVT = cms.OutputModule("PoolOutputModule",
     outputCommands = cms.untracked.vstring('drop *', 'keep *_MEtoEDMConverter_*_*'),
     fileName = cms.untracked.string("HcalValHarvestingEDM.root")
)


process.hcalDigiAnalyzer = cms.EDAnalyzer("HcalDigisValidation",
    outputFile		      = cms.untracked.string('HcalDigisValidationRelVal.root'),
#    digiLabel                = cms.InputTag("simHcalDigis"),
#    subdetector               = cms.untracked.string('HE'),
    digiLabel   = cms.InputTag("hcalDigis"),  # regular collections
#--- Two Upgrade (doSLHC=True) collections
    digiLabelHBHE = cms.InputTag("simHcalDigis","HBHEUpgradeDigiCollection"), 
    digiLabelHF = cms.InputTag("simHcalDigis","HFUpgradeDigiCollection"),
    zside		      = cms.untracked.string('*'),
    mode		      = cms.untracked.string('multi'),
    hcalselector	      = cms.untracked.string('all'),
    mc			      = cms.untracked.string('yes'), # 'yes' for MC
    doSLHC                    = cms.untracked.bool(True) #  False for SLHC and True for regular rel val 
)   

process.hcalTowerAnalyzer = cms.EDAnalyzer("CaloTowersValidation",
    outputFile               = cms.untracked.string('CaloTowersValidationRelVal.root'),
    CaloTowerCollectionLabel = cms.untracked.InputTag('towerMaker'),

    hcalselector             = cms.untracked.string('all'),
    mc                       = cms.untracked.string('no'),
    useAllHistos             = cms.untracked.bool(False)                         
)

process.hcalNoiseRates = cms.EDAnalyzer('NoiseRates',
    outputFile   = cms.untracked.string('NoiseRatesRelVal.root'),
    rbxCollName  = cms.untracked.InputTag('hcalnoise'),

    minRBXEnergy = cms.untracked.double(20.0),
    minHitEnergy = cms.untracked.double(1.5),
    useAllHistos = cms.untracked.bool(False)                         
)


#--- NB: CHANGED for SLHC/Upgrade
process.hcalRecoAnalyzer = cms.EDAnalyzer("HcalRecHitsValidation",
    outputFile                = cms.untracked.string('HcalRecHitValidationRelVal.root'),

### Upgrade 2019
    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbheUpgradeReco"),
    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfUpgradeReco"),
### 2017
#    HBHERecHitCollectionLabel = cms.untracked.InputTag("hbhereco"),
#    HFRecHitCollectionLabel   = cms.untracked.InputTag("hfreco"),
#####
    HORecHitCollectionLabel   = cms.untracked.InputTag("horeco"),
    eventype                  = cms.untracked.string('multi'),
    ecalselector              = cms.untracked.string('yes'),
    hcalselector              = cms.untracked.string('all'),
    mc                        = cms.untracked.string('no'),
    doSLHC                    = cms.untracked.bool(True) #  True for SLHC and False for regular rel val 
)



process.load('Configuration/StandardSequences/EDMtoMEAtRunEnd_cff')
process.dqmSaver.referenceHandling = cms.untracked.string('all')

cmssw_version = os.environ.get('CMSSW_VERSION','CMSSW_X_Y_Z')
Workflow = '/HcalValidation/'+'Harvesting/'+str(cmssw_version)
process.dqmSaver.workflow = Workflow

process.calotowersClient = cms.EDAnalyzer("CaloTowersClient", 
     outputFile = cms.untracked.string('CaloTowersHarvestingME.root'),
     DQMDirName = cms.string("/") # root directory
)

process.noiseratesClient = cms.EDAnalyzer("NoiseRatesClient", 
     outputFile = cms.untracked.string('NoiseRatesHarvestingME.root'),
     DQMDirName = cms.string("/") # root directory
)

process.hcalrechitsClient = cms.EDAnalyzer("HcalRecHitsClient", 
     outputFile = cms.untracked.string('HcalRecHitsHarvestingME.root'),
     DQMDirName = cms.string("/") # root directory
)


process.hcaldigisClient = cms.EDAnalyzer("HcalDigisClient",
     outputFile	= cms.untracked.string('HcalDigisHarvestingME.root'),
     DQMDirName	= cms.string("/") # root directory
)   


#process.hcalDigis.InputLabel = 'rawDataCollector' # MC
#---------------------------------------------------- Job PATH 
process.p2 = cms.Path( 
#process.hcalTowerAnalyzer * 
#process.hcalNoiseRates * 
process.hcalRecoAnalyzer *
#process.hcalDigiAnalyzer * 
#process.calotowersClient * 
#process.noiseratesClient *
#process.hcalrechitsClient * 
#process.hcaldigisClient * 
process.dqmSaver)


#--- Customization for SLHC

from SLHCUpgradeSimulations.Configuration.HCalCustoms import customise_HcalPhase1
process=customise_HcalPhase1(process)

