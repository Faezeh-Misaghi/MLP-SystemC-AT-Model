#ifndef CPU_MEM_IF_H
#define CPU_MEM_IF_H

#include <systemc>
#include <vector>

using namespace sc_core;
using namespace std;

class CPU_Mem_IF : public sc_interface {
public:
    virtual vector<int> get_layer_sizes() = 0;
    virtual vector<float> get_input_vector() = 0;
    virtual void wait_layer_ready() = 0;
    virtual void wait_input_ready() = 0;
    virtual void ack_layer_received() = 0;
    virtual void ack_input_received() = 0;
    virtual void set_layer_sizes(const vector<int>& layers) = 0;
    virtual void set_input_vector(const vector<float>& input) = 0;
    virtual void set_output_vector(const vector<float>& output) = 0;
    virtual vector<float> get_final_output() = 0;
    virtual void ack_output_received() = 0;
    virtual void wait_output_ack() = 0;
    virtual bool has_output() = 0;
    virtual void reset_output_flag() = 0 ;
};

#endif
