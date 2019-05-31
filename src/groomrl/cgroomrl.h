/**
 * GroomRL C API
 * This file is part of GroomRL by S. Carrazza and F. A. Dreyer
 */

// load model from file
extern void read_groomrl_model(const char* folder);

// apply model to input data
extern bool apply_groomrl_model(double *vars);
