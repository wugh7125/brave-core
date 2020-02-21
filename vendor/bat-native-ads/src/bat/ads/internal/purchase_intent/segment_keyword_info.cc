/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/purchase_intent/segment_keyword_info.h"

namespace ads {

SegmentKeywordInfo::SegmentKeywordInfo() :
    segments({}),
    keywords("") {}

SegmentKeywordInfo::SegmentKeywordInfo(
    const std::vector<std::string>& segments,
    const std::string& keywords) :
    segments(segments),
    keywords(keywords) {}

SegmentKeywordInfo::SegmentKeywordInfo(const SegmentKeywordInfo& info) :
    segments(info.segments),
    keywords(info.keywords) {}

SegmentKeywordInfo::~SegmentKeywordInfo() {}

}  // namespace ads
