/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_SEGMENT_KEYWORD_INFO_H_
#define BAT_ADS_INTERNAL_SEGMENT_KEYWORD_INFO_H_

#include <string>
#include <vector>

namespace ads {

struct SegmentKeywordInfo {
 public:
  SegmentKeywordInfo();
  SegmentKeywordInfo(
      const std::vector<std::string>& segments,
      const std::string& keywords);
  SegmentKeywordInfo(const SegmentKeywordInfo& info);
  ~SegmentKeywordInfo();

  std::vector<std::string> segments;
  std::string keywords;
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_SEGMENT_KEYWORD_INFO_H_
