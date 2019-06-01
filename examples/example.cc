// To run this example, use the following command:
//
//   ./example < data/single-event.dat
//
//----------------------------------------------------------------------
// $Id$
//
// Copyright (c) 2019-, Stefano Carrazza, Frederic Dreyer
//
//----------------------------------------------------------------------
// This file is part of FastJet contrib.
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#include <iostream>
#include <sstream>

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include <sstream>
#include "groomrl/groomrl.h"

using namespace std;
using namespace fastjet;

// forward declaration to make things clearer
void read_event(vector<PseudoJet> &event);

ostream & operator<<(ostream &, const PseudoJet &);

//----------------------------------------------------------------------
int main(){

  //----------------------------------------------------------
  // read in input particles
  vector<PseudoJet> event;
  read_event(event);
  cout << "# read an event with " << event.size() << " particles" << endl;

  //----------------------------------------------------------
  // set up GroomRL 
  string folder = "models/groomRL-W";
  GroomRL groomRL(folder);
  cout << "GroomRL: " << groomRL.description() << endl;

  // get some anti-kt jets
  double R = 1.0, ptmin = 100.0;
  JetDefinition jet_def(antikt_algorithm, R);
  ClusterSequence cs(event, jet_def);
  vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets(ptmin));

  // go over the jets and groom them
  for (unsigned ijet = 0; ijet < jets.size(); ijet++) {
    PseudoJet groomRL_jet = groomRL(jets[ijet]);
    // print the original and groomed jet kinematics
    cout << endl;
    cout << "original jet: " << jets[ijet]  << endl;
    cout << "GroomRL  jet: " << groomRL_jet << endl;
  }

  return 0;
}

// read in input particles
void read_event(vector<PseudoJet> &event){  
  string line;
  while (getline(cin, line)) {
    istringstream linestream(line);
    // take substrings to avoid problems when there is extra "pollution"
    // characters (e.g. line-feed).
    if (line.substr(0,4) == "#END") {return;}
    if (line.substr(0,1) == "#") {continue;}
    double px,py,pz,E;
    linestream >> px >> py >> pz >> E;
    PseudoJet particle(px,py,pz,E);

    // push event onto back of full_event vector
    event.push_back(particle);
  }
}

//----------------------------------------------------------------------
/// overloaded jet info output
ostream & operator<<(ostream & ostr, const PseudoJet & jet) {
  if (jet == 0) {
    ostr << " 0 ";
  } else {
    ostr << " pt = " << jet.pt()
         << " m = " << jet.m()
         << " y = " << jet.rap()
         << " phi = " << jet.phi();
  }
  return ostr;
}
