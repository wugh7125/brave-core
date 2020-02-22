/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "navigator_user_agent.h"

#define getUserAgent getUserAgent_ChromiumImpl
#include "../../../../../../../third_party/blink/renderer/core/frame/navigator_user_agent.cc"
#undef getUserAgent

namespace blink {

ScriptPromise NavigatorUserAgent::getUserAgent(ScriptState* script_state) {
  auto* resolver = MakeGarbageCollected<ScriptPromiseResolver>(script_state);
  ScriptPromise promise = resolver->Promise();

  blink::UserAgent* idl_metadata = blink::UserAgent::Create();

  idl_metadata->setBrand(String::FromUTF8("Brave"));
  resolver->Resolve(idl_metadata);

  return promise;
}

}  // namespace blink
