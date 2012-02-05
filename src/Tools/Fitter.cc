#include "Fitter.h"

#include <gear/GEAR.h>
#include <marlin/Global.h>
#include "FTrackTools.h"
#include <algorithm>

using namespace FTrack;

MarlinTrk::IMarlinTrkSystem* Fitter::_trkSystem = NULL;

void Fitter::init(){
   
   
   _trkSystem =  MarlinTrk::Factory::createMarlinTrkSystem( "KalTest" , marlin::Global::GEAR , "" ) ;
   
   // set the options   
   _trkSystem->setOption( MarlinTrk::IMarlinTrkSystem::CFG::useQMS,        true ) ;       //multiple scattering
   _trkSystem->setOption( MarlinTrk::IMarlinTrkSystem::CFG::usedEdx,       true) ;     //energy loss
   _trkSystem->setOption( MarlinTrk::IMarlinTrkSystem::CFG::useSmoothing,  false) ;    //smoothing
   
   // initialise the tracking system
   _trkSystem->init() ;
   
   
   
   
}

Fitter::Fitter( Track* track ){
   
 
   _trackerHits = track->getTrackerHits();
   
   fit();
   
   
}

Fitter::Fitter( std::vector < TrackerHit* > trackerHits ){
   
   _trackerHits = trackerHits;
   
   fit();
   
}


void Fitter::fit(){
   
 
      
   if ( Fitter::_trkSystem == NULL ) Fitter::init(); //if it is not already initialise, do it now
   

   //Create a new MarlinTrack
   MarlinTrk::IMarlinTrack* marlin_trk = _trkSystem->createTrack();
   
   
   // sort the hits
   std::sort( _trackerHits.begin(), _trackerHits.end(), compare_TrackerHit_z );
   
   // now at [0] is the hit with the smallest |z| and at [1] is the one with a bigger |z| and so on
   // So the direction of the hits when following the index from 0 on is:
   // from inside out: from the IP into the distance. 
   // (It is important to keep in mind, in which direction we fit, when using MarlinTrk)
   
   
   
   //add hits to the MarlinTrk (in the same sorted order --> marlin_trk is from inside to the outside as well
   for( unsigned i=0 ; i < _trackerHits.size() ; i++ )
   {
      
      marlin_trk->addHit( _trackerHits.at(i) );
      
   }
   
   
   marlin_trk->initialise( MarlinTrk::IMarlinTrack::backward ) ; 
   //Here we first needed the order. We initialize the track to be fitted from outside in
   // As our track is sorted from inside out, this means, that we go backwards 
   //(compared to the order the hits are stored in the track)
   
   
   marlin_trk->fit() ; //fit the track


   const gear::Vector3D IP(0.,0.,0.); // nominal IP
   
   
   // propagate to the IP
   TrackStateImpl* trkState = new TrackStateImpl() ;
   
   trkState->setLocation( TrackState::AtIP ) ;
   
   marlin_trk->propagate(IP, *trkState, _chi2, _Ndf ) ;
   

   
   
   
   delete marlin_trk;


   
   
   
}
   
   
