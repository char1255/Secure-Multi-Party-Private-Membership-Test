#include "core/crc/crc64.hpp"

uint64_t mpmt::crc64::compute(const uint8_t* const data, const size_t len)
{
    uint64_t crc_reg = m_mask;
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t index = (crc_reg ^ data[i]) & 0xFF;
        crc_reg = (crc_reg >> 8) ^ m_table[index];
    }
    return crc_reg ^ m_mask;
}