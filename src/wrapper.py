# This file is part of GroomRL by S. Carrazza and F. A. Dreyer
# GroomRL C API definitions
from cgroomrl import ffi
from keras.models import model_from_json
import numpy as np
import json


def select_action(q_values):
        """GreedyQPolicy:
        Return the selected action
        # Arguments
            q_values (np.ndarray): List of the estimations of Q for each action
        # Returns
            Selection action
        """
        assert q_values.ndim == 1
        action = np.argmax(q_values)
        return action

@ffi.def_extern()
def read_groomrl_model(folder):
    """Load groomer from file"""
    folder = ffi.string(folder).decode('utf-8')
    """Read in a runcard json file and set up dimensions correctly."""
    with open(folder + '/runcard.json','r') as f:
        res = json.load(f)
    # set the dimension and build the network
    global state_dim, model
    state_dim = res.get('groomer_env')["state_dim"]
    with open(folder + '/model.json') as f:
        architecture = json.load(f)
    model = model_from_json(architecture)
    model.load_weights(folder + '/weights.h5')

def asarray(ffi, ptr, length):
    """Auxiliary function for cdata to python conversion"""
    T = ffi.getctype( ffi.typeof(ptr).item )    
    return np.frombuffer(ffi.buffer(ptr, length*ffi.sizeof(T) ), np.dtype('f8'))

@ffi.def_extern()
def apply_groomrl_model(state):
    """Return an action given an input state"""
    state= asarray(ffi, state, 5)
    # load global model
    global state_dim, model
    q_values = model.predict_on_batch(np.array([[state[:state_dim]]])).flatten()
    action   = select_action(q_values=q_values)
    # return true if branch is to be removed
    return (action==1)
