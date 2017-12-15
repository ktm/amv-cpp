//
// Created by ktm on 2/4/16.
//

#ifndef AMVMODEL_CHAI_SCRIPT_H
#define AMVMODEL_CHAI_SCRIPT_H

#include <chaiscript/chaiscript.hpp>
#include <mutex>
#include <boost/variant.hpp>
#include "../collection/name_value_pair.hxx"


class chaiscript_gateway {
    std::mutex data_mutex;
    double toNumber(std::string arg);
    chaiscript_gateway();

public:
    chaiscript::ChaiScript js_context;
    static chaiscript_gateway& Instance() {
        static chaiscript_gateway INSTANCE;
        return INSTANCE;
    }

    bool evaluate_condition(std::string condition);
    void call_function(std::string fxn);
    void write_to_js(std::string name, Value value);
    void write_from_js(std::string name, chaiscript::Boxed_Value v);

    std::string createCallString(std::string fxnname, std::vector<std::string> args);

    //TIL template fxn defs go in the header, otherwise the linker fails
    template<typename ValueType> ValueType call(std::string fxnname, std::vector<std::string> args) {
        const std::string callString = createCallString(fxnname, args);
        chaiscript::Boxed_Value bv = chaiscript_gateway::Instance().js_context.eval(callString);

        auto info = bv.get_type_info();

        return chaiscript::boxed_cast<ValueType>(bv);
    }

    template<typename Ret, typename ... Param>
    void registerFunction(Ret (*func)(Param...), std::string name) {
        js_context.add(chaiscript::fun(func), name);
    }

    chaiscript::Boxed_Value evaluateFile(std::string fname) {
        return js_context.eval_file(fname);
    }
};


#endif //AMVMODEL_CHAI_SCRIPT_H
