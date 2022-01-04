#include "mac.h"
#include <Cocoa/Cocoa.h>

void showDockIcon(bool show) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  ProcessSerialNumber psn;
  windowCounter += show ? 1 : -1;
  UInt32 transformState =
      show ? kProcessTransformToForegroundApplication : kProcessTransformToUIElementApplication;
  const bool shouldHideLastWindow =
      (transformState == kProcessTransformToUIElementApplication && windowCounter == 0) ? true
                                                                                        : false;
  const bool shouldShowWindow =
      (transformState == kProcessTransformToForegroundApplication && windowCounter > 0) ? true
                                                                                        : false;
  if (GetCurrentProcess(&psn) == noErr && (shouldShowWindow || shouldHideLastWindow)) {
    TransformProcessType(&psn, transformState);
    dispatch_async(dispatch_get_main_queue(), ^{
      [NSApp activateIgnoringOtherApps:YES];
    });
  }
#pragma clang diagnostic pop
}
