// -*- C++ -*-
//
// Package:    Analysis/JetMassStudies
// Class:      JetMassStudies
// 
/**\class JetMassStudies JetMassStudies.cc Analysis/JetMassStudies/plugins/JetMassStudies.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Salvatore Rappoccio
//         Created:  Tue, 25 Jul 2017 14:16:25 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"

#include "DataFormats/Math/interface/deltaR.h"

//
// class declaration
//

class JetMassStudies : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit JetMassStudies(const edm::ParameterSet&);
      ~JetMassStudies();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<pat::JetCollection>                     ak8jetToken_;
      edm::EDGetTokenT<std::vector<reco::VertexCompositePtrCandidate> >      svToken_;
      double coneSize_;
      TH2D * ak8charge;
      TH2D * ks_mass;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JetMassStudies::JetMassStudies(const edm::ParameterSet& iConfig) :
  ak8jetToken_(consumes<pat::JetCollection>( iConfig.getParameter<edm::InputTag>("ak8src") ) ),
  svToken_(consumes<std::vector<reco::VertexCompositePtrCandidate> >( iConfig.getParameter<edm::InputTag>("svsrc") ) ),
  coneSize_( iConfig.getParameter<double>("coneSize") )
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   edm::Service<TFileService> fs;
   ks_mass = fs->make<TH2D>("ks_mass" , "K_{S} mass within AK8 jets" , 100, 0, 1000, 50 , 0.4 , 0.6 );
}


JetMassStudies::~JetMassStudies()
{ 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
JetMassStudies::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle<std::vector<reco::VertexCompositePtrCandidate> > vertices; 
  iEvent.getByToken( svToken_, vertices );

  edm::Handle<std::vector<pat::Jet> > ak8jets;
  iEvent.getByToken(ak8jetToken_, ak8jets);

  for (const pat::Jet &j : *ak8jets) {
    // // To get the constituents of the AK8 jets, you have to loop over all of the
    // // daughters recursively. To save space, the first two constituents are actually
    // // the Soft Drop SUBJETS, which will then point to their daughters.
    // // The remaining constituents are those constituents removed by soft drop but
    // // still in the AK8 jet.
    // std::vector<reco::Candidate const *> constituents;
    // for ( unsigned ida = 0; ida < j.numberOfDaughters(); ++ida ) {
    //   reco::Candidate const * cand = j.daughter(ida);
    //   if ( cand->numberOfDaughters() == 0 )
    // 	constituents.push_back( cand ) ;
    //   else {
    // 	for ( unsigned jda = 0; jda < cand->numberOfDaughters(); ++jda ) {
    // 	  reco::Candidate const * cand2 = cand->daughter(jda);
    // 	  constituents.push_back( cand2 );
    // 	}
    //   }
    // }

    for( auto const & vtx : *vertices ) {
      if ( reco::deltaR2( vtx.p4(), j.p4() ) < coneSize_*coneSize_ && vtx.charge() == 0 && vtx.numberOfDaughters() == 2 ) {
	ks_mass->Fill( j.pt(), vtx.mass() );
      }
      
    }

  

    
    //std::sort( constituents.begin(), constituents.end(), [] (reco::Candidate const * ida, reco::Candidate const * jda){return ida->pt() > jda->pt();} );
  }
  


}


// ------------ method called once each job just before starting event loop  ------------
void 
JetMassStudies::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
JetMassStudies::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
JetMassStudies::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setComment("EDAnalyzer to check into jet mass studies");
  desc.add<edm::InputTag>("ak8src", edm::InputTag("no default") )->setComment("input collection for ak8 jets");
  desc.add<edm::InputTag>("svsrc", edm::InputTag("no default") )->setComment("input collection for secondary vertices");
  desc.add<double>("coneSize", 0.8 )->setComment("Delta R matching cone size between vertices and jets");
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetMassStudies);
