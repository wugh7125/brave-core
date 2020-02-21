/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_INTENT_SIGNAL_INFO_H_
#define BAT_ADS_INTERNAL_INTENT_SIGNAL_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct IntentSignalInfo {
 public:
  IntentSignalInfo();
  IntentSignalInfo(
      const std::vector<std::string> segments, // TODO(MH): reference?
      const std::uint16_t& strength);
  IntentSignalInfo(const IntentSignalInfo& info);
  ~IntentSignalInfo();

  std::vector<std::string> segments;
  std::uint16_t strength;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_INTENT_SIGNAL_INFO_H_
