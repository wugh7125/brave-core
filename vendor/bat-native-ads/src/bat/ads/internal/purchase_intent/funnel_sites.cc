/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/purchase_intent/funnel_sites.h"

#include "url/gurl.h"

#include <iostream>

namespace ads {

FunnelSites::FunnelSites() = default;
FunnelSites::~FunnelSites() = default;

bool FunnelSites::IsFunnelSite(const std::string& url) {
  auto visited_url = GURL(url);

  if (!visited_url.has_host()) {
    return false;
  }

  auto is_funnel_site = false;

  for (const auto& funnel_site : _funnel_sites) {
    auto funnel_site_hostname = GURL(funnel_site.url_netloc);

    if (!funnel_site_hostname.is_valid()) {
      continue;
    }

    if (visited_url.DomainIs(funnel_site_hostname.host_piece())) {
      is_funnel_site = true;
      break;
    }
  }

  return is_funnel_site;
}

}  // namespace ads
