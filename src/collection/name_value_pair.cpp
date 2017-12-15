//
// Created by ktm on 2/21/16.
//
#include "name_value_pair.hxx"

int BLANK_VALUE  = 0;
int STRING_VALUE = 1;
int INT_VALUE    = 2;
int DOUBLE_VALUE = 3;

bool value_compare(Value& lhs, Value& rhs) {
    int lhst = lhs.which();
    int rhst = rhs.which();

    if ((lhst == BLANK_VALUE) && (rhst == BLANK_VALUE)) {
        return true;
    }

    if ((lhst == BLANK_VALUE) && (rhst > BLANK_VALUE)) {
        return false;
    }

    if ((rhst == BLANK_VALUE) && (lhst > BLANK_VALUE)) {
        return false;
    }

    if ((lhst == STRING_VALUE) && (rhst == STRING_VALUE)) {
        return (boost::get<std::string>(lhs).compare(boost::get<std::string>(rhs)) == 0);
    }

    if ((lhst == INT_VALUE) && (rhst == INT_VALUE)) {
        return (boost::get<int>(lhs) == boost::get<int>(rhs));
    }

    if ((lhst == DOUBLE_VALUE) && (rhst == DOUBLE_VALUE)) {
        return (boost::get<double>(lhs) == boost::get<double>(rhs));
    }
    return false;
}
