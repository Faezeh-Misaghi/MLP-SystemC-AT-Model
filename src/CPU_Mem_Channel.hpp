#ifndef CPU_MEM_CHANNEL_H
#define CPU_MEM_CHANNEL_H

#include <systemc>
#include <vector>
#include <fstream>
#include "CPU_Mem_IF.hpp"

using namespace std;
using namespace sc_core;

class CPU_Mem_Channel : public sc_module, public CPU_Mem_IF {
public:
    SC_CTOR(CPU_Mem_Channel) {}

    vector<int> layer_sizes;
    vector<float> input_vector;
    vector<float> output_result;
    bool output_available = false;

    sc_event ev_layer_ready, ev_input_ready;
    sc_event ev_layer_ack, ev_input_ack;
    sc_event output_ready_event;  
    sc_event output_ack_event; 

    void set_layer_sizes(const vector<int>& layers) {
        layer_sizes = layers;
        ev_layer_ready.notify(SC_ZERO_TIME);
    }

    void set_input_vector(const vector<float>& input) {
        input_vector = input;
        ev_input_ready.notify(SC_ZERO_TIME);
    }

    void wait_layer_ready() override {
        wait(ev_layer_ready);
    }

    void wait_input_ready() override {
        wait(ev_input_ready);
    }

    void ack_layer_received() override {
        ev_layer_ack.notify(SC_ZERO_TIME);
    }

    void ack_input_received() override {
        ev_input_ack.notify(SC_ZERO_TIME);
    }

    vector<int> get_layer_sizes() override {
        wait(ev_layer_ack);
        return layer_sizes;
    }

    vector<float> get_input_vector() override {
        wait(ev_input_ack);
        return input_vector;
    }

    vector<float> get_final_output() {
        return output_result;
    }

    void wait_layer_ack() {
        wait(ev_layer_ack);
    }

    void wait_input_ack() {
        wait(ev_input_ack);
    }

    void set_output_vector(const vector<float>& output) override {
        output_result = output;
        output_available = true;
        output_ready_event.notify(SC_ZERO_TIME);
    }

    bool has_output() override {
        return output_available;
    }

    void ack_output_received() override {
        output_ack_event.notify(SC_ZERO_TIME);
    }

    void wait_output_ack() {
        wait(output_ack_event);
    }

    void reset_output_flag() override {
        output_available = false;
    }
};

#endif