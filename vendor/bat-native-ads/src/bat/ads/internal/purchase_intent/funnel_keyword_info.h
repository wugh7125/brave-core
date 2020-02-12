/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_FUNNEL_KEYWORD_INFO_H_
#define BAT_ADS_INTERNAL_FUNNEL_KEYWORD_INFO_H_

#include <string>

namespace ads {

struct FunnelKeywordInfo {
 public:
  FunnelKeywordInfo();
  FunnelKeywordInfo(
    const std::string& keywords,
    const std::uint8_t& weight);
  FunnelKeywordInfo(const FunnelKeywordInfo& info);
  ~FunnelKeywordInfo();

  std::string keywords;
  std::uint8_t weight;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_FUNNEL_KEYWORD_INFO_H_
