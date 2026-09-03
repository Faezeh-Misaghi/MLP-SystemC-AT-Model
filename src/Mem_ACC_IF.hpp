#ifndef MEM_ACC_IF_H
#define MEM_ACC_IF_H

#include <systemc>
#include <vector>

using namespace sc_core;
using namespace std;

class MemACC_IF : public sc_interface {
public:
    virtual void request_layer(int layer_idx, int in_size, int out_size) = 0;
    virtual void wait_data_ready() = 0;
    virtual vector<float> get_weights() = 0;
    virtual vector<float> get_biases() = 0;
};

#endif
