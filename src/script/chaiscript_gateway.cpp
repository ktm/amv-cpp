//
// Created by ktm on 2/4/16.
//

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include "chaiscript_gateway.h"
#include "../gps/gps.hxx"
#include "../model/context.hxx"

chaiscript_gateway::chaiscript_gateway(): js_context(chaiscript::Std_Lib::library()) { }

bool chaiscript_gateway::evaluate_condition(std::string condition) {
    std::cout << "conditional: " << condition << std::endl;
    std::lock_guard<std::mutex> data_lock(data_mutex);
    bool retval = js_context.eval<bool>(condition);
    return retval;
}

void chaiscript_gateway::call_function(std::string fxn) {
    js_context.eval(fxn);
}

double chaiscript_gateway::toNumber(std::string arg) {
    double retval = HUGE_VAL;

    char * endptr;
    double d = strtod(arg.c_str(), &endptr);
    if (arg.c_str() != endptr) {
        retval = d;
    }
    return retval;
}

void chaiscript_gateway::write_to_js(std::string name, Value value) {
    chaiscript::Boxed_Value v;

    if (value.which() == INT_VALUE) {
        v = chaiscript::Boxed_Value(boost::get<int>(value));
    } else if (value.which() == DOUBLE_VALUE) {
        v = chaiscript::Boxed_Value(boost::get<double >(value));
    } else if (value.which() == STRING_VALUE) {
        v = chaiscript::Boxed_Value(boost::get<std::string>(value));
    }
    js_context.set_global(v, name);
}

std::string chaiscript_gateway::createCallString(std::string fxnname, std::vector<std::string> args) {
    std::string callString(fxnname);
    callString.append("(");

    int n = args.size();
    for (int idx = 0; idx < n; ++idx) {
        Value value = (Context::Instance().read(args[idx]));
        if (value.which() == INT_VALUE) {
            callString.append(to_string(boost::get<int>(value)));
        } else if (value.which() == DOUBLE_VALUE) {
            callString.append(to_string(boost::get<double>(value)));
        } else if (value.which() == STRING_VALUE) {
            callString.append("\"");
            callString.append(boost::get<std::string>(value));
            callString.append("\"");
        }
        if (idx < n-1) {
            callString.append(",");
        }
    }
    callString.append(");");
    std::cout << callString << std::endl;
    return callString;
}

void chaiscript_gateway::write_from_js(std::string name, chaiscript::Boxed_Value v) {

    if (v.get_type_info().is_arithmetic()) {
        try {
            Context::Instance().write(name, chaiscript::boxed_cast<double >(v));
        } catch (...) {
            Context::Instance().write(name, chaiscript::boxed_cast<int>(v));
        }
    } else {
        Context::Instance().write(name, chaiscript::boxed_cast<std::string>(v));
    }
}

