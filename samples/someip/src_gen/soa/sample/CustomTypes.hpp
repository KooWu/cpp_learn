/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.2.15.v202410041619.
* Used org.franca.core 0.13.1.201807231814.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef SOA_SAMPLE_Custom_Types_HPP_
#define SOA_SAMPLE_Custom_Types_HPP_



#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#define HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <CommonAPI/Types.hpp>
#include <cstdint>

#if defined (HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE)
#undef COMMONAPI_INTERNAL_COMPILATION
#undef HAS_DEFINED_COMMONAPI_INTERNAL_COMPILATION_HERE
#endif

namespace soa {
namespace sample {

struct CustomTypes {
    struct PlusParamInfo : CommonAPI::Struct< int16_t, int16_t> {
    
        PlusParamInfo()
        {
            std::get< 0>(values_) = 0;
            std::get< 1>(values_) = 0;
        }
        PlusParamInfo(const int16_t &_x, const int16_t &_y)
        {
            std::get< 0>(values_) = _x;
            std::get< 1>(values_) = _y;
        }
        inline const int16_t &getX() const { return std::get< 0>(values_); }
        inline void setX(const int16_t &_value) { std::get< 0>(values_) = _value; }
        inline const int16_t &getY() const { return std::get< 1>(values_); }
        inline void setY(const int16_t &_value) { std::get< 1>(values_) = _value; }
        inline bool operator==(const PlusParamInfo& _other) const {
        return (getX() == _other.getX() && getY() == _other.getY());
        }
        inline bool operator!=(const PlusParamInfo &_other) const {
            return !((*this) == _other);
        }
    
    };
    struct TimeInfo : CommonAPI::Struct< int64_t, int64_t> {
    
        TimeInfo()
        {
            std::get< 0>(values_) = 0;
            std::get< 1>(values_) = 0;
        }
        TimeInfo(const int64_t &_timestamp, const int64_t &_seq)
        {
            std::get< 0>(values_) = _timestamp;
            std::get< 1>(values_) = _seq;
        }
        inline const int64_t &getTimestamp() const { return std::get< 0>(values_); }
        inline void setTimestamp(const int64_t &_value) { std::get< 0>(values_) = _value; }
        inline const int64_t &getSeq() const { return std::get< 1>(values_); }
        inline void setSeq(const int64_t &_value) { std::get< 1>(values_) = _value; }
        inline bool operator==(const TimeInfo& _other) const {
        return (getTimestamp() == _other.getTimestamp() && getSeq() == _other.getSeq());
        }
        inline bool operator!=(const TimeInfo &_other) const {
            return !((*this) == _other);
        }
    
    };


static inline const char* getTypeCollectionName() {
    static const char* typeCollectionName = "soa.sample.CustomTypes";
    return typeCollectionName;
}


}; // struct CustomTypes

} // namespace sample
} // namespace soa

namespace CommonAPI {
}


namespace std {
}



#endif // SOA_SAMPLE_Custom_Types_HPP_
