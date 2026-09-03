#ifndef CPU_ACC_IF_HPP
#define CPU_ACC_IF_HPP

#include <systemc>
#include <vector>

using namespace std;
using namespace sc_core;

class CPU_ACC_IF : public sc_interface {
public:
    virtual void send_start() = 0;
    virtual void wait_done() = 0;
    virtual void wait_ready() = 0;

    virtual void set_layer_sizes(const vector<int>&) = 0;
    virtual void set_input_vector(const vector<float>&) = 0;
    virtual vector<float> get_output_vector() = 0;

    // ✅ Add these to fix your errors:
    virtual void wait_for_start() = 0;
    virtual void notify_done() = 0;
    virtual void notify_ready() = 0;
    virtual void set_output_vector(const vector<float>&) = 0;

    virtual const vector<int>& get_layer_sizes() const = 0;
    virtual const vector<float>& get_input_vector() const = 0;

};

#endif
