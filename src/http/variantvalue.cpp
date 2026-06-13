#include "variantvalue.h"

using namespace http;

VariantValue::VariantValue() {}

VariantValue::VariantValue(std::string v)
    : m_stringValue(std::move(v))
{}

int VariantValue::toInt() const
{
    int result{0};

    try {
        result = std::stoi(m_stringValue);
    } catch (...) {
    }

    return result;
}

double VariantValue::toDouble() const
{
    double result{0.0};

    try {
        result = std::stod(m_stringValue);
    } catch (...) {
    }

    return result;
}

std::string VariantValue::toString() const
{
    return m_stringValue;
}

long VariantValue::toLong() const
{
    long result{0};

    try {
        result = std::stol(m_stringValue);
    } catch (...) {
    }

    return result;
}
