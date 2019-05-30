#include <string>
#include <queue>
#include <vector>
#include "groomrl/groomrl.h"
#include "fastjet/tools/Recluster.hh"

using namespace fastjet;

GroomRLconfig* GroomRLconfig::instance = 0;
std::string GroomRLconfig::folder = "";

void GroomRLconfig::setup(const std::string& path) {
  if (instance==0) {
    // save the folder and set up the keras model
    folder = path;
    instance = new GroomRLconfig();
    read_groomrl_model(path.c_str());
  } else {
    throw Error("GroomRL was initiated more than once");
  }
}

PseudoJet GroomRL::result(const PseudoJet & jet) const {
  // start by reclustering jet with C/A algorithm
  PseudoJet ca_jet = Recluster(cambridge_algorithm, JetDefinition::max_allowable_R)(jet);

  // create a priority queue containing the subjets and a comparison definition
  std::priority_queue<PseudoJet, std::vector<PseudoJet>, CompareJetsWithDeltaRsqr>  subjets;

  // initialize counter to 1 subjet (i.e. the full ca_jet)
  subjets.push(ca_jet);
  int counter = 1;
  // initialize max value for subjets
  int max_njet = ca_jet.constituents().size();
  
  // loop over C/A tree
  PseudoJet piece1, piece2;
  while (counter<max_njet) {
    if (subjets.top().has_parents(piece1,piece2)) {
      if (piece2.pt2() > piece1.pt2()) std::swap(piece1,piece2); 
      bool groom_action = groom_step(piece1, piece2);
      if (!groom_action) {
	subjets.pop();
	subjets.push(piece1);
	subjets.push(piece2);
	++counter;
      } else {
	subjets.pop();
	subjets.push(piece1);
	max_njet -= piece2.constituents().size();
      }
    }
  }
  // now create and fill the pieces vector with all the subjets
  std::vector<PseudoJet> pieces(subjets.size());
  std::copy(&(subjets.top()), &(subjets.top()) + subjets.size(), &pieces[0]);
  
  // create the final result jet
  PseudoJet result = join(pieces);

  return result;
}

bool GroomRL::groom_step(const PseudoJet & j1, const PseudoJet & j2) const {
  double delta = j1.delta_R(j2);
  double z = j2.pt()/(j1.pt() + j2.pt());
  double lnm = 0.5*log(abs((j1 + j2).m2()));
  double lnKt = log(j2.pt()*delta);
  double lnz = log(z);
  double lnDelta = log(delta);
  double psi = atan((j1.rap() - j2.rap())/(j1.phi() - j2.phi()));
  // now set up the state
  double *state = new double[5];
  state[0] = lnz;
  state[1] = lnDelta;
  state[2] = psi;
  state[3] = lnm;
  state[4] = lnKt;
  return apply_groomrl_model(state);
}

std::string GroomRL::description() const {
  return GroomRLconfig::description();
}
