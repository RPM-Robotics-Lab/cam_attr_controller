#include <iostream>
#include <algorithm>

#include "mvIMPACT_CPP/mvIMPACT_acquire.h"
#include "bluefox2_enumerator.h"
#include "Bluefox2Config.h"

using namespace std;

namespace bluefox2 {
using namespace mvIMPACT::acquire;

// Convinient typedefs
template <typename ValueType>
using TranslationDict = std::vector<std::pair<std::string, ValueType>>;


template <typename T1, typename T2>
T1 Clamp(const T1& value, const T2& low, const T2& high) {
  return max<T1>(low, min<T1>(high, value));
}

template <typename PropertyType, typename ValueType>
ValueType ClampProperty(const PropertyType& prop, const ValueType& value) {
  return Clamp(value, prop.getMinValue(), prop.getMaxValue());
}

template <typename PropertyType>
TranslationDict<typename PropertyType::value_type> GetTranslationDict(
    const PropertyType& prop) {
  TranslationDict<typename PropertyType::value_type> dict;
  prop.getTranslationDict(dict);
  return dict;
}

template <typename ValueType>
void PrintTranslationDict(const TranslationDict<ValueType>& dict) {
  for (const auto& p : dict) {
    cout << "[" << p.first << ": " << p.second << "]";
  }
  cout << endl;
}

int PixelFormatToEncoding(const TImageBufferPixelFormat& pixel_format);

int BayerPatternToEncoding(const TBayerMosaicParity& bayer_pattern,
                                   int bytes_per_pixel);

double PixelClockToFrameRate(int pclk_khz, double width, double height,
                             double expose_us);

template <typename Prop>
void PrintProperty(const Prop& prop) {
  cout << "Property name: " << prop.displayName() << "\n";
  if (prop.hasMaxValue()) {
    cout << "  Max value: " << prop.getMaxValue() << "\n";
  }
  if (prop.hasMinValue()) {
    cout << "  Min value: " << prop.getMinValue() << "\n";
  }
  if (prop.hasStepWidth()) {
    cout << "  Step width: " << prop.getStepWidth() << "\n";
  }
  cout << "  Writable: " << prop.isWriteable() << "\n";
  cout << "  Visible: " << prop.isVisible() << "\n";
  cout << "  Valid: " << prop.isValid() << endl;

  // TODO: improve error display
}

// TODO: improve error display
template <typename PropertyType, typename ValueType>
void WriteProperty(const PropertyType& prop, ValueType value) {
    using PropertyValueType = typename PropertyType::value_type;
    // Check if it's possible to write to this property
    if (!(prop.isVisible() && prop.isWriteable() && prop.isValid())) {
        cout << prop.name() << ": unable to write to property" << endl;
    return;
    }

    // Clamp value to valid range only if it's not an enum
    if (!std::is_enum<PropertyValueType>::value) {
        if (prop.hasMaxValue() && prop.hasMinValue()) {
            value = ClampProperty(prop, value);
        }
    }

    try {
        prop.write(static_cast<PropertyValueType>(value));
    } 
    catch (...) {
        cout << prop.name() << ": failed to write to property" << endl;
        PrintTranslationDict(GetTranslationDict(prop));
    }
}

template <typename PropertyType, typename ValueType>
void ReadProperty(const PropertyType& prop, ValueType& value) {
    if (!(prop.isValid() && prop.isVisible())) {
        cout << prop.name() << ": unable to read from property" << endl;
        return;
    }

    try {
        value = static_cast<ValueType>(prop.read());
    }
    catch (...) {
        cout << prop.name() << ": failed to read from property" << endl;
    }
}

template <typename PropertyType, typename ValueType>
void WriteAndReadProperty(const PropertyType& prop, ValueType& value) {
    WriteProperty(prop, value);
    ReadProperty(prop, value);
}

}