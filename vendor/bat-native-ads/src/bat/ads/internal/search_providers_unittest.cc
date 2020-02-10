/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

#include "testing/gtest/include/gtest/gtest.h"

#include "bat/ads/internal/search_providers.h"

using ::testing::_;

// npm run test -- brave_unit_tests --filter=AdsSearchProviders*

namespace {

const std::vector<std::tuple<std::string, bool>> kTestSearchQueries = {
  std::tuple<std::string, bool>("https://duckduckgo.com/", false),
  std::tuple<std::string, bool>("https://duckduckgo.com/?q=this+is+another+test+search&t=h_&ia=videos", true),
  std::tuple<std::string, bool>("https://www.google.com/preferences?hl=en-GB&fg=1", false),
  std::tuple<std::string, bool>("https://www.google.com/search?q=this+is+a+test+search&oq=this+is+a+test+search&aqs=chrome..69i57.2258j0j1&sourceid=chrome&ie=UTF-8", true)
};

}  // namespace

namespace ads {

class AdsSearchProvidersTest : public ::testing::Test {
 protected:
  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  AdsSearchProvidersTest() :
      mock_ads_client_(std::make_unique<MockAdsClient>()),
      ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~AdsSearchProvidersTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
    search_providers_ = std::make_unique<SearchProviders>(); // TODO(MH): use as abstract class
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case
  std::unique_ptr<SearchProviders> search_providers_;
};

TEST_F(AdsSearchProvidersTest, IsSearchEngine) {  
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(true);
}

TEST_F(AdsSearchProvidersTest, ExtractsSearchQueryKeywords) {  
  // // Arrange
  // auto test_search_query = "https://duckduckgo.com/?q=this+is+another+test+search&t=h_&ia=videos"

  // // Act
  // const bool did_match = funnel_sites_->IsFunnelSite(url);
  // search_providers_->

  // // TODO(MH): Delete
  // std::cout << "DEBUG: " << url << " - " << is_funnel_site << " - "
  //     << did_match << "\n";

  // // Assert
  EXPECT_TRUE(true);
}


}  // namespace ads
