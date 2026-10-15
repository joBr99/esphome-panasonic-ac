#include "components/panasonic_ac/panasonic_ac_cnt_power.h"

#include <cassert>
#include <vector>

int main() {
  using esphome::panasonic_ac::cnt::decode_power;
  // Captured response from a real AC, off: 5 W standby.
  std::vector<uint8_t> packet{0x70, 0x20, 0x40, 0x28, 0x80, 0xA0, 0xF9, 0x40, 0x00,
                            0x40, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00,
                            0x19, 0x12, 0x35, 0x19, 0x12, 0xFF, 0x80, 0x80, 0xFF,
                            0x80, 0x05, 0x00, 0x00, 0x80, 0x81, 0x25, 0x13};
  assert(decode_power(packet.data(), packet.size()) == 5.0f);
  for (size_t size = 0; size < 32; ++size) {
    std::vector<uint8_t> short_packet(size);
    assert(std::isnan(decode_power(short_packet.data(), size)));
  }
  packet[28] = 0xE8;
  packet[29] = 0x03;
  packet[30] = 10;
  assert(decode_power(packet.data(), packet.size()) == 990.0f);
  packet[28] = 0;
  packet[29] = 0;
  packet[30] = 0;
  assert(decode_power(packet.data(), packet.size()) == 0.0f);
  packet[30] = 1;
  assert(std::isnan(decode_power(packet.data(), packet.size())));
  // Avoid signed 16-bit overflow in the decoder.
  packet[28] = 0xFF;
  packet[29] = 0xFF;
  packet[30] = 0;
  assert(decode_power(packet.data(), packet.size()) == 65535.0f);
}
