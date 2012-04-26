#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


/**
 * This small executable is there to automically run Marlin with some parameter altered in steps
 * and add the information of the changes to a file
 */

int main(int argc,char *argv[]){

   
   const std::string STEERING_FILE = "/scratch/ilcsoft/MyMarlinSteers/background.xml";
   
   std::vector< std::string > filesToWriteParam; //in those files the changed parameter is written
   filesToWriteParam.push_back( "/scratch/ilcsoft/MyMarlinSteers/Feedback/TrackingFeedbackForwardSum.csv" );
   filesToWriteParam.push_back( "/scratch/ilcsoft/MyMarlinSteers/Feedback/TrackingFeedbackSiliconSum.csv" );
   
   const std::string processorName = "MyFTDBackgroundProcessor";
   const std::string paramName = "DensityRegulator";
   const double paramMin = 0.;
   const double paramMax = 1.;
   const double paramStep = 0.2;
   
   for( double param = paramMin; param <= paramMax; param+= paramStep ){
      
      
      
      // set the parameters for Marlin
      std::stringstream parameters;
      parameters << " --" << processorName << "." << paramName << "=" << param ;
      
      
      
      
      /**********************************************************************************************/
      /*                Run Marlin                                                                  */
      /**********************************************************************************************/
      std::string command = "Marlin " + STEERING_FILE + " " + parameters.str();
      int returnValue = system( command.c_str() );
      
      if( returnValue != 0 ){
         
         std::cout << "\n\n Marlin did not return 0. Error!!!\n\n";
         return 1;
         
      }
      
      
      for( unsigned i=0; i < filesToWriteParam.size(); i++ ){
         
         std::ofstream myfile;
         myfile.open (filesToWriteParam[i].c_str() , std::ios::app);
         myfile << processorName << "." << paramName << "\t" << param << "\t\t";
         myfile.close();
         
      }
      
      
   }
   
   
   return 0;

}