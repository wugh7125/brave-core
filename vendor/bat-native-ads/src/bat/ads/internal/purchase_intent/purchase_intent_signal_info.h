/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PURCHASE_INTENT_SIGNAL_INFO_H_
#define BAT_ADS_INTERNAL_PURCHASE_INTENT_SIGNAL_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct PurchaseIntentSignalInfo {
 public:
  PurchaseIntentSignalInfo();
  PurchaseIntentSignalInfo(
    const uint64_t timestamp_in_seconds,
    const std::vector<std::string> segments, // TODO(Moritz Haller): Pass as reference?
    const std::uint16_t& weight);
  PurchaseIntentSignalInfo(const PurchaseIntentSignalInfo& info);
  ~PurchaseIntentSignalInfo();

  uint64_t timestamp_in_seconds;
  std::vector<std::string> segments;
  std::uint16_t weight;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PURCHASE_INTENT_SIGNAL_INFO_H_
