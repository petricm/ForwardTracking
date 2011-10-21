#ifndef Crit2_DeltaPhi_h
#define Crit2_DeltaPhi_h


#include "ICriterion.h"

namespace FTrack{

   /** Criterion: 
    */
   class Crit2_DeltaPhi : public ICriterion{



   public:
      
      Crit2_DeltaPhi ( double deltaPhiMin , double deltaPhiMax );
      
      virtual bool areCompatible( Segment* parent , Segment* child )throw( BadSegmentLength );

      virtual ~Crit2_DeltaPhi(){};

    
   private:
      
      double _deltaPhiMax;
      double _deltaPhiMin;
      
      
      
   };

}

#endif

