#include "ip_container.h"

#include <sstream>

IPContainer::IPContainer(uint8_t first, uint8_t second, uint8_t third, uint8_t forth) {
  data_[0] = first;
  data_[1] = second;
  data_[2] = third;
  data_[3] = forth;
}

const uint8_t *IPContainer::getData() const {
  return &data_[0];
}

std::string IPContainer::toString() const {
  std::stringstream out_strm;
  out_strm << (int) data_[0] << "." << (int)  data_[1] << "." << (int) data_[2] << "." << (int) data_[3];
  return out_strm.str();
}

bool IPContainer::operator==(const IPContainer &rhs) const {
  auto l_data = data_;
  auto r_data = rhs.getData();
  return (l_data[0] == r_data[0]) && (l_data[1] == r_data[1]) && (l_data[2] == r_data[2]) && (l_data[3] == r_data[3]);
}

bool IPContainer::operator!=(const IPContainer &rhs) const {
  return !(*this==rhs);
}

uint8_t IPContainer::operator[](int index) const {
  return data_[index];
}

//IPContainer::IPContainer(std::string data) {
//
//}
