#!/usr/bin/python

import sys, os

# This is a dictionary of flags to pull out the datasets of interest mapped to the desired name from the hcal script
dsFlags = {'RelValTTbar_':'TTbar', 'RelValQCD_Pt_80_120_':'QCD', 'RelValQCD_Pt_3000_3500_':'HighPtQCD', 'RelValMinBias_':'MinBias'}
# Dataset to select (simply so we only get one of each sample above)
sds = "GEN-SIM-RECO"
# filename prefix 
fnPrefix = "DQM_V0001_R000000001"
# blank curl command 
curl = "/usr/bin/curl -O -L --capath %(CERT_DIR)s --key %(USER_PROXY)s --cert %(USER_PROXY)s https://cmsweb.cern.ch/dqm/relval/data/browse/ROOT/RelVal/%(relvalDIR)s/%(fname)s"
# output file name blank
ofnBlank = "HcalRecHitValidationRelVal_%(sample)s_Startup_%(label)s.root"
# default release file for MC stub
dfTextFile = "%s_standard.txt"

# ensure all required parameters are included
if len(sys.argv) < 2:
    print "Script needs 2 input variables: RelValDataSetList.txt fullReleaseName"
    print "RelValDataSetList.txt : text file from relval announcement"
    print "fullReleaseName : CMSSW_5_2_0_pre3"
    exit(0)

# gather input parameter
label     = sys.argv[1]
filein = ""
if len(sys.argv) > 2:
    filein = sys.argv[2]
else:
    filein = dfTextFile%label
    print "Taking filenames form file %s"%filein

# retrieve the list of datasets
if not os.path.isfile(filein):
    os.system("wget http://cms-project-relval.web.cern.ch/cms-project-relval/relval_stats/%s"%filein)

# modify label to shortened format (remove CMSSW and '_')
slabel = label.replace('CMSSW','').replace("_","")

# get relval dir from label
clabel = label.split("_")
relvalDIR = "%s_%s_%s_x"%(clabel[0], clabel[1], clabel[2])

# initialize voms proxy for transfer
os.system('voms-proxy-init -voms cms')

# gather necessary proxy info for curl
X509_CERT_DIR = os.getenv('X509_CERT_DIR', "/etc/grid-security/certificates")
X509_USER_PROXY = os.getenv('X509_USER_PROXY')

# open raw inpout file 
fin = open(filein, "r")

# loop over file and pull out lines of interest
for line in fin:
    # limit to one entry per dataset
    if sds in line:
        # select datasets of interest
        for str in dsFlags.keys():
            if str in line:
                # extract dataset path
                path = line.split('|')[1].strip()
                # construct file name
                fname = fnPrefix + path.replace("/","__").replace(sds, "DQMIO.root")
                # copy file with curl
                curlCommand = curl%{"CERT_DIR":X509_CERT_DIR,"USER_PROXY":X509_USER_PROXY, "relvalDIR":relvalDIR,"fname":fname}
                print curlCommand
                os.system(curlCommand)
                # rename file for use with hcal scripts
                ofn = ofnBlank%{"sample":dsFlags[str],"label":slabel}
                mvCommand = "mv %(fn)s %(ofn)s"%{"fn":fname,"ofn":ofn}
                print mvCommand
                os.system(mvCommand)
                print ""

# Copy the single pion scan part from Salavat's directory
cpCommand = "cp /afs/cern.ch/user/a/abdullin/public/pi50_scan/pi50scan%s_ECALHCAL_CaloTowers.root ."%slabel
print cpCommand
os.system(cpCommand)
print ""
