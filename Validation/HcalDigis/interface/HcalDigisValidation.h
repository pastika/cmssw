#ifndef HCALDIGISVALIDATION_H
#define	HCALDIGISVALIDATION_H

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"

#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"


#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

//includes for HGCal
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/FCalGeometry/interface/HGCalGeometry.h"
#include "DataFormats/ForwardDetId/interface/HGCalDetId.h"
#include "DataFormats/ForwardDetId/interface/HGCHEDetId.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "DataFormats/HGCDigi/interface/HGCDigiCollections.h"

#include <map>
#include <vector>
#include <utility>
#include <ostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>

typedef std::pair<int, int> subdetector;
static const subdetector SUB_NULL(0, 0);
static const subdetector SUB_HB(4, 1);
static const subdetector SUB_HE(4, 2);
static const subdetector SUB_HO(4, 3);
static const subdetector SUB_HF(4, 4);
static const subdetector SUB_HGCHEB(6, 5);


class HcalDigisValidation : public edm::EDAnalyzer {
public:
    explicit HcalDigisValidation(const edm::ParameterSet&);

    ~HcalDigisValidation() {
    };

private:

    struct HistLim {

        HistLim(int nbin, double mini, double maxi)
        : n(nbin), min(mini), max(maxi) {
        }
        int n;
        double min;
        double max;
    };

    virtual void analyze(const edm::Event&, const edm::EventSetup&);

    virtual void beginJob();

    virtual void endJob();

    void beginRun();

    void endRun();

    DQMStore* dbe_;
    std::map<std::string, MonitorElement*> *msm_;

    void book1D(std::string name, int n, double min, double max);

    void book1D(std::string name, HistLim limX);

    void fill1D(std::string name, double X, double weight = 1);

    void book2D(std::string name, HistLim limX, HistLim limY);

    void fill2D(std::string name, double X, double Y, double weight = 1);

    void bookPf(std::string name, HistLim limX, HistLim limY);

    void fillPf(std::string name, double X, double Y);

    MonitorElement* monitor(std::string name);

    void booking(std::string subdetopt, bool bnoise, bool bmc);

    std::string str(int x);

    template<class Digi> void reco(const edm::Event& iEvent, const edm::EventSetup& iSetup, std::string subdet);

    void eval_occupancy();

    std::string outputFile_;
    std::string subdet_;
    std::string zside_;
    std::string dirName_;
    edm::InputTag inputTag_;
    std::string mode_;
    std::string mc_;
    std::string hgchebGeomName_;
    int noise_;
    bool doSLHC_;
    bool doHGC_;

    // specifically for SLHC    
    edm::InputTag inputTagHBHE_;  
    edm::InputTag inputTagHF_;

    // Tag for HGCAL
    edm::InputTag inputTagHGCHEB_;

    edm::ESHandle<CaloGeometry> geometry_;
    edm::ESHandle<HGCalGeometry> hgchebGeom_;
    edm::ESHandle<HcalDbService> conditions_;
    
    std::map<std::string, int> nevents_;

    int nevtot = 0;

    // Det numbering: 4 - HCAL, 6 - Forward
    // HCAL subdet numbering: 1 - HB, 2 - HE, 3 - HO, 4 - HF
    // HGC subdet numbering 3 - EE, 4 - HEF, 5 - HEB

    class DigiSummary
    {
    public:
        DetId cell;
        int depth;
        int iphi;
        int ieta;
        double eta;
        double phi;
        subdetector sub;
        int capId[10];
    };

    template<class DigiCollectionIter> void getDigiSummary(DigiCollectionIter& digiItr, DigiSummary *ds);
    template<class Digi> void getCalibDigi(const Digi& ucalDigi, CaloSamples* tool);

};

#endif


