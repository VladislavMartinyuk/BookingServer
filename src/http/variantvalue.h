#ifndef VARIANTVALUE_H
#define VARIANTVALUE_H

#include <string>

namespace http {

class VariantValue
{
public:
    VariantValue();
    VariantValue(std::string v);

    int toInt() const;
    double toDouble() const;
    std::string toString() const;
    long toLong() const;

private:
    std::string m_stringValue;
};

} // namespace http

#endif // VARIANTVALUE_H
