#ifndef CPU_ACC_CHANNEL_HPP
#define CPU_ACC_CHANNEL_HPP

#include <systemc>
#include "CPU_ACC_IF.hpp"

using namespace sc_core;
using namespace std;

class CPU_ACC_Channel : public sc_module, public CPU_ACC_IF {
public:
    SC_CTOR(CPU_ACC_Channel) {}

    // Sync
    sc_event start_event, done_event, ready_event;

    // Data
    vector<int> layer_sizes;
    vector<float> input_vector, output_vector;

    // Control flags
    bool start_flag = false, done_flag = false, ready_flag = false;

    // CPU to Accelerator
    void send_start() override {
        start_flag = true;
        start_event.notify(SC_ZERO_TIME);
    }

    void set_layer_sizes(const vector<int>& layers) override {
        layer_sizes = layers;
    }

    void set_input_vector(const vector<float>& input) override {
        input_vector = input;
    }

    // Accelerator to CPU
    void wait_done() override {
        while (!done_flag) wait(done_event);
        done_flag = false;
    }

    void wait_ready() override {
        while (!ready_flag) wait(ready_event);
        ready_flag = false;
    }

    void notify_done() {
        done_flag = true;
        done_event.notify(SC_ZERO_TIME);
    }

    void notify_ready() {
        ready_flag = true;
        ready_event.notify(SC_ZERO_TIME);
    }

    void wait_for_start() {
        while (!start_flag) wait(start_event);
        start_flag = false;
    }

    void set_output_vector(const vector<float>& output) {
        output_vector = output;
    }

    vector<float> get_output_vector() override {
        return output_vector;
    }

    const vector<int>& get_layer_sizes() const { return layer_sizes; }
    const vector<float>& get_input_vector() const { return input_vector; }
};

#endif
