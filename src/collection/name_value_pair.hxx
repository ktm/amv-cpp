//
// Created by ktm on 9/11/15.
//
#include <memory>
#include <string>
#include <boost/variant.hpp>

#ifndef AMVMODEL_NAME_VALUE_PAIR_HXX_H
#define AMVMODEL_NAME_VALUE_PAIR_HXX_H

using Value = boost::variant<boost::blank, std::string, int, double>;
using NameValuePair = std::pair<std::string, Value>;
using NameValuePairPtr = std::shared_ptr<NameValuePair>;

extern int BLANK_VALUE;
extern int STRING_VALUE;
extern int INT_VALUE;
extern int DOUBLE_VALUE;

bool value_compare(Value& lhs, Value& rhs);
#endif //AMVMODEL_NAME_VALUE_PAIR_HXX_H
