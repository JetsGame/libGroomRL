// This file is part of GroomRL by S. Carrazza and F. A. Dreyer
#include <iostream>
#include "groomrl/cgroomrl.h"
using namespace std;

int main() {

    // Step 1: loading model from folder
    read_groomrl_model("../GroomRL/results/groomerW_final");

    // Step 2: apply apply
    double *x = new double[5];
    x[0] = 1;
    x[1] = 2;
    x[2] = 3;
    x[3] = 4;
    x[4] = 5;
    x[5] = 6;
    bool s = apply_groomrl_model(x);
    cout << s << endl;
}
