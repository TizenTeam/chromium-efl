// Copyright 2014 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "browser_main_parts_efl.h"
#include "base/logging.h"

#include "devtools_delegate_efl.h"

#include "base/base_switches.h"
#include "base/command_line.h"
#include "content/public/common/content_switches.h"


namespace content {

BrowserMainPartsEfl::BrowserMainPartsEfl()
  : devtools_delegate_(NULL) {
}

BrowserMainPartsEfl::~BrowserMainPartsEfl() {
}

void BrowserMainPartsEfl::PreMainMessageLoopRun() {
  // PreMainMessageLoopRun is called just before the main message loop is run.
  // This is to create DevToolsDelegateEfl instance at PreMainMessageLoopRun
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kRemoteDebuggingPort))
    devtools_delegate_ = new DevToolsDelegateEfl();
}

void BrowserMainPartsEfl::PostMainMessageLoopRun() {
  if (devtools_delegate_)
    devtools_delegate_->Stop();
}

}  // namespace
