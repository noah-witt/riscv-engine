/**
 * @file cli.cpp
 * @author Noah Witt naqitt18@ole.augie.edu
 * @brief provides a command line interface to use this tool.
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "boost/program_options.hpp" 
#include "boost/log/trivial.hpp"
#include "boost/log/core.hpp"
#include <boost/log/expressions.hpp>
#include <iostream> 
#include "../start.hpp"

int main(int argc, char** argv) {
    boost::program_options::options_description desc("Options");
    std::string help = "Select the file with the \"--input\" option.\nUse the \"--v\" option to print debug output.\nUse the \"--maxInst\" option to specify the maximum number of instructions to execute (default 100000).\nUse the \"--d\" option to run with the debugger.\n";
    desc.add_options() 
        ("help", "Print help")
        ("input", boost::program_options::value<std::string>(), "the input file name")
        ("v", "debug output")
        ("vv", "print some info then quit without running the program")
        ("d", "load with the debugger")
        ("maxInst", boost::program_options::value<int>(),  "the max number of instructions to run");
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    if(vm.count("help")) {
        std::cout << help;
        return 0;
    }
    if(!vm.count("input")) {
        std::cout << "file name is required\n\n";
        std::cout << help;
        return 0;
    }
    std::string inputFileName = vm["input"].as<std::string>();
    int maxSteps = 100000;
    if(vm.count("maxInst")) {
        maxSteps = vm["maxInst"].as<int>();
    }
    if(!vm.count("v")) {
        boost::log::core::get()->set_filter (
        boost::log::trivial::severity >= boost::log::trivial::warning
    );

    }
    if(vm.count("vv")) {
        std::cout << "RISC_V runtime\n";
        std::cout <<"fileName: "<<inputFileName<<"\n";
        return 0;
    }
    bool debug = false;
    if(vm.count("d")) debug = true;
    if(debug) maxSteps = 0;
    start(inputFileName, maxSteps, debug);
    std::cout<<"\n";
}
