/**
 * GroomRL C API
 * This file is part of GroomRL by S. Carrazza and F. A. Dreyer
 */

#ifndef __GROOMRL_H__
#define __GROOMRL_H__

#include "fastjet/tools/Transformer.hh"

// load model from file
extern void read_groomrl_model(const char* folder);

// apply model to input data
extern bool apply_groomrl_model(double *vars);

//----------------------------------------------------------------------
/// \class GroomRLconfig
/// singleton class to set up the GroomRL configuration
class GroomRLconfig {
public:
  /// create pointer instance of GroomRLconfig and setup the keras model
  static void setup(const std::string& folder);

  /// description
  static std::string description() {
    if (instance==0) throw Error("GroomRL was not initiated");
    return folder;
  }

  /// destructor
  ~GroomRLconfig(){}

private:
  /// constructor
  GroomRLconfig() {}
  static std::string folder;
  static GroomRLconfig* instance;
};
  
//------------------------------------------------------------------------
/// \class GroomRL
/// Grooming algorithm using a pre-trained keras model to recursively
/// prune the clustering tree
class GroomRL : public fastjet::Transformer {
public:
  /// default ctor
  GroomRL(const std::string& folder) {
    GroomRLconfig::setup(folder);
  }

  // definitions needed for comparison of subjets
  struct CompareJetsWithDeltaRsqr {
    // return the squared Delta R value between the two subjets
    double jet_deltaRsqr(const fastjet::PseudoJet& jet) const {
      fastjet::PseudoJet piece1, piece2;
      if (jet.has_parents(piece1,piece2))
	return piece1.squared_distance(piece2);
      return 0.0;
    }
    
    bool operator ()(const fastjet::PseudoJet& j1,
		     const fastjet::PseudoJet& j2) const {
      return jet_deltaRsqr(j1) < jet_deltaRsqr(j2);
    }
  };

  // inherited from the Transformer base
  //----------------------------------------------------------------------
  /// apply the grooming algorithm
  virtual fastjet::PseudoJet result(const fastjet::PseudoJet & j) const;

  /// description of the tool
  virtual std::string description() const;

  /// default dtor
  ~GroomRL(){}
private:
  /// decide on the action for the current grooming step
  bool groom_step(const fastjet::PseudoJet & j1,
		  const fastjet::PseudoJet & j2) const;
};

#endif // __GROOMRL_H__
