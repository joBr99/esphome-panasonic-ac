#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace esphome {
namespace panasonic_ac {
namespace cnt {

// Call only for a poll response whose header, length and checksum were verified.
inline float decode_power(const uint8_t *packet, size_t size) {
  // Offsets 28/29 hold little-endian watts, offset 30 is subtracted.
  // Require a separate trailing checksum, not just the three data bytes.
  if (size < 32)
    return NAN;
  const int watts = static_cast<int>(packet[28]) + (static_cast<int>(packet[29]) << 8) - packet[30];
  return watts >= 0 ? static_cast<float>(watts) : NAN;
}

}  // namespace cnt
}  // namespace panasonic_ac
}  // namespace esphome
