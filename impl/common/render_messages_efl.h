// Copyright 2014 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Multiply-included file, no traditional include guard.

#include "base/values.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_channel_handle.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/referrer.h"
#include "ui/gfx/ipc/gfx_param_traits.h"
#include "API/ewk_hit_test_private.h"
#include "API/ewk_text_style_private.h"
#include "cache_params_efl.h"
#include "navigation_policy_params.h"
#include "renderer/print_pages_params.h"
#include "third_party/WebKit/public/web/WebNavigationPolicy.h"
#include "third_party/WebKit/public/web/WebNavigationType.h"
#include "third_party/WebKit/public/web/WebViewModeEnums.h"
#include "ui/gfx/ipc/gfx_param_traits.h"
#include "url/gurl.h"

#include "tizen_webview/public/tw_content_security_policy.h"
#include "tizen_webview/public/tw_error.h"
#include "tizen_webview/public/tw_hit_test.h"
#include "tizen_webview/public/tw_settings.h"
#include "tizen_webview/public/tw_view_mode.h"
#include "tizen_webview/public/tw_wrt.h"

#include <string>
#include <map>

#if defined(TIZEN_MULTIMEDIA_SUPPORT)
#include "base/file_descriptor_posix.h"
#include "base/memory/shared_memory.h"
#endif

typedef std::map<std::string, std::string> StringMap;

//-----------------------------------------------------------------------------
// RenderView messages
// These are messages sent from the browser to the renderer process.

// these messages belong to "chrome messages" in chromium
// we can add our own class for them but then we need to patch ipc/ipc_message_start.h
// so using same message class for these messages.
// but actual messages are different so they shouldn't mess with chrome messages
#define IPC_MESSAGE_START ChromeMsgStart

IPC_STRUCT_TRAITS_BEGIN(CacheParamsEfl)
  IPC_STRUCT_TRAITS_MEMBER(cache_total_capacity)
  IPC_STRUCT_TRAITS_MEMBER(cache_min_dead_capacity)
  IPC_STRUCT_TRAITS_MEMBER(cache_max_dead_capacity)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(tizen_webview::ContentSecurityPolicyType)

IPC_ENUM_TRAITS(blink::WebNavigationPolicy)
IPC_ENUM_TRAITS(blink::WebNavigationType)

IPC_STRUCT_TRAITS_BEGIN(NavigationPolicyParams)
  IPC_STRUCT_TRAITS_MEMBER(render_view_id)
  IPC_STRUCT_TRAITS_MEMBER(cookie)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(httpMethod)
  IPC_STRUCT_TRAITS_MEMBER(referrer)
  IPC_STRUCT_TRAITS_MEMBER(policy)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(should_replace_current_entry)
  IPC_STRUCT_TRAITS_MEMBER(is_main_frame)
  IPC_STRUCT_TRAITS_MEMBER(is_redirect)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(SelectionColor)
  IPC_STRUCT_TRAITS_MEMBER(r)
  IPC_STRUCT_TRAITS_MEMBER(g)
  IPC_STRUCT_TRAITS_MEMBER(b)
  IPC_STRUCT_TRAITS_MEMBER(a)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(SelectionStylePrams)
  IPC_STRUCT_TRAITS_MEMBER(underline_state)
  IPC_STRUCT_TRAITS_MEMBER(italic_state)
  IPC_STRUCT_TRAITS_MEMBER(bold_state)
  IPC_STRUCT_TRAITS_MEMBER(bg_color)
  IPC_STRUCT_TRAITS_MEMBER(color)
  IPC_STRUCT_TRAITS_MEMBER(font_size)
  IPC_STRUCT_TRAITS_MEMBER(order_list_state)
  IPC_STRUCT_TRAITS_MEMBER(un_order_list_state)
  IPC_STRUCT_TRAITS_MEMBER(text_align_center_state)
  IPC_STRUCT_TRAITS_MEMBER(text_align_left_state)
  IPC_STRUCT_TRAITS_MEMBER(text_align_right_state)
  IPC_STRUCT_TRAITS_MEMBER(text_align_full_state)
  IPC_STRUCT_TRAITS_MEMBER(has_composition)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(tizen_webview::Hit_Test_Mode)
IPC_ENUM_TRAITS(tizen_webview::Hit_Test_Result_Context)

IPC_STRUCT_TRAITS_BEGIN(_Ewk_Hit_Test::Hit_Test_Node_Data)
  IPC_STRUCT_TRAITS_MEMBER(tagName)
  IPC_STRUCT_TRAITS_MEMBER(nodeValue)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(_Ewk_Hit_Test::Hit_Test_Image_Buffer)
  IPC_STRUCT_TRAITS_MEMBER(fileNameExtension)
  IPC_STRUCT_TRAITS_MEMBER(imageBitmap)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(_Ewk_Hit_Test)
  IPC_STRUCT_TRAITS_MEMBER(context)
  IPC_STRUCT_TRAITS_MEMBER(linkURI)
  IPC_STRUCT_TRAITS_MEMBER(linkLabel)
  IPC_STRUCT_TRAITS_MEMBER(linkTitle)
  IPC_STRUCT_TRAITS_MEMBER(imageURI)
  IPC_STRUCT_TRAITS_MEMBER(mediaURI)
  IPC_STRUCT_TRAITS_MEMBER(isEditable)
  IPC_STRUCT_TRAITS_MEMBER(mode)
  IPC_STRUCT_TRAITS_MEMBER(nodeData)
// XXX: find a better way to prevent the crash when copying SkBitmap
#if !defined(EWK_BRINGUP)
  IPC_STRUCT_TRAITS_MEMBER(imageData)
#endif
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(DidPrintPagesParams)
  IPC_STRUCT_TRAITS_MEMBER(metafile_data_handle)
  IPC_STRUCT_TRAITS_MEMBER(data_size)
  IPC_STRUCT_TRAITS_MEMBER(document_cookie)
  IPC_STRUCT_TRAITS_MEMBER(filename)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tizen_webview::WrtIpcMessageData)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(value)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(reference_id)
IPC_STRUCT_TRAITS_END()

IPC_ENUM_TRAITS(blink::WebViewMode)

IPC_STRUCT_TRAITS_BEGIN(tizen_webview::Settings)
  IPC_STRUCT_TRAITS_MEMBER(javascript_can_open_windows)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(tizen_webview::Error)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(is_main_frame)
  IPC_STRUCT_TRAITS_MEMBER(code)
  IPC_STRUCT_TRAITS_MEMBER(description)
  IPC_STRUCT_TRAITS_MEMBER(domain)
IPC_STRUCT_TRAITS_END()


// Tells the renderer to clear the cache.
IPC_MESSAGE_CONTROL0(EflViewMsg_ClearCache)
IPC_MESSAGE_ROUTED0(EwkViewMsg_UseSettingsFont)
IPC_MESSAGE_ROUTED0(EwkViewMsg_SetBrowserFont)
IPC_MESSAGE_ROUTED0(EwkViewMsg_SuspendScheduledTask)
IPC_MESSAGE_ROUTED0(EwkViewMsg_ResumeScheduledTasks)
IPC_MESSAGE_CONTROL1(EflViewMsg_SetCache,
                     CacheParamsEfl)

// Tells the renderer to dump as much memory as it can, perhaps because we
// have memory pressure or the renderer is (or will be) paged out.  This
// should only result in purging objects we can recalculate, e.g. caches or
// JS garbage, not in purging irreplaceable objects.
IPC_MESSAGE_CONTROL0(EwkViewMsg_PurgeMemory)

IPC_MESSAGE_CONTROL4(EwkViewMsg_SetWidgetInfo,
                     int,            // result: widgetHandle
                     double,         // result: scale
                     std::string,    // result: theme
                     std::string)    // result: encodedBundle

IPC_MESSAGE_CONTROL1(EwkViewMsg_SendWrtMessage,
                     tizen_webview::WrtIpcMessageData /* data */);

IPC_MESSAGE_ROUTED0(EwkViewMsg_GetSelectionStyle)

IPC_MESSAGE_ROUTED2(EwkViewMsg_SetCSP,
                    std::string, /* policy */
                    tizen_webview::ContentSecurityPolicyType /* header type */)

IPC_MESSAGE_ROUTED2(EwkViewMsg_SetScroll,
                    int, /* horizontal position */
                    int /* vertical position */)

IPC_MESSAGE_ROUTED3(EwkViewMsg_DoHitTest,
                    int, /* horizontal position */
                    int, /* vertical position */
                    tizen_webview::Hit_Test_Mode /* mode */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_DidFailLoadWithError,
                    tizen_webview::Error /* error */)

IPC_MESSAGE_ROUTED4(EwkViewMsg_DoHitTestAsync,
                    int, /* horizontal position */
                    int, /* vertical position */
                    tizen_webview::Hit_Test_Mode, /* mode */
                    int64_t /* request id */)

IPC_MESSAGE_ROUTED3(EwkViewMsg_PrintToPdf,
                    int, /* width */
                    int, /* height */
                    base::FilePath /* file name to save pdf*/)

IPC_MESSAGE_ROUTED1(EflViewMsg_UpdateSettings, tizen_webview::Settings)

// from renderer to browser

IPC_MESSAGE_ROUTED1(EwkHostMsg_DidPrintPagesToPdf,
                    DidPrintPagesParams /* pdf document parameters */)

IPC_SYNC_MESSAGE_CONTROL1_1(EwkHostMsg_DecideNavigationPolicy,
                           NavigationPolicyParams,
                           bool /*handled*/)

IPC_SYNC_MESSAGE_ROUTED0_2(EwkHostMsg_GetContentSecurityPolicy,
                           std::string, /* policy */
                           tizen_webview::ContentSecurityPolicyType /* header type */)

IPC_SYNC_MESSAGE_ROUTED1_1(EwkHostMsg_WrtSyncMessage,
                           tizen_webview::WrtIpcMessageData /* data */,
                           std::string /*result*/);

IPC_MESSAGE_ROUTED3(EwkViewMsg_Scale,
                    double, /* scale factor */
                    int, /* center x */
                    int /* center y */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_PlainTextGet,
                    int /* callback id */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_PlainTextGetContents,
                    std::string, /* contentText */
                    int /* callback id */)

IPC_MESSAGE_ROUTED1(EwkHostMsg_WrtMessage,
                    tizen_webview::WrtIpcMessageData /* data */);

IPC_MESSAGE_ROUTED2(EwkHostMsg_DidChangeContentsSize,
                    int, /* width */
                    int /* height */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_SelectionTextStyleState,
                    SelectionStylePrams /* params */)

IPC_MESSAGE_ROUTED2(EwkViewMsg_SelectClosestWord,
                    int, /* x */
                    int /* y */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_DidChangeMaxScrollOffset,
                    int, /*max scrollX*/
                    int  /*max scrollY*/)

IPC_MESSAGE_ROUTED2(EwkHostMsg_DidChangeScrollOffset,
                    int, /*scrollX*/
                    int  /*scrollY*/)

IPC_MESSAGE_CONTROL3(EwkViewHostMsg_HitTestReply,
                    int, /* render_view_id */
                    _Ewk_Hit_Test, /* Ewk Hit test data without node map */
                    NodeAttributesMap /* node attributes */)

IPC_MESSAGE_CONTROL4(EwkViewHostMsg_HitTestAsyncReply,
                    int, /* render_view_id */
                    _Ewk_Hit_Test, /* Ewk Hit test data without node map */
                    NodeAttributesMap, /* node attributes */
                    int64_t /* request id */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_GetMHTMLData,
                    int /* callback id */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_ReadMHTMLData,
                    std::string, /* Mhtml text */
                    int /* callback id */)

IPC_MESSAGE_ROUTED1(EwkHostMsg_DidChangePageScaleFactor,
                    double /* page scale factor */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_DidChangePageScaleRange,
                    double, /* minimum page scale factor */
                    double  /* maximum page scale factor */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_SetDrawsTransparentBackground,
                    bool /* enabled */)

// Notifies the browser to form submit
IPC_MESSAGE_ROUTED1(EwkHostMsg_FormSubmit, GURL)

IPC_MESSAGE_ROUTED1(EwkViewMsg_WebAppIconUrlGet,
                    int /* callback id */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_WebAppIconUrlGet,
                    std::string, /* icon url */
                    int /* callback id */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_WebAppIconUrlsGet,
                    int /* callback id */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_WebAppIconUrlsGet,
                    StringMap, /* icon urls */
                    int /* callback id */)

IPC_MESSAGE_ROUTED1(EwkViewMsg_WebAppCapableGet,
                    int /* calback id */)

IPC_MESSAGE_ROUTED2(EwkHostMsg_WebAppCapableGet,
                    bool, /* capable */
                    int /* calback id */)

// Used to set view mode.
IPC_MESSAGE_ROUTED1(ViewMsg_SetViewMode,
                    blink::WebViewMode /* view_mode */)

#if defined(TIZEN_MULTIMEDIA_SUPPORT)
// This message runs the GStreamer for decoding audio for webaudio.
IPC_MESSAGE_CONTROL3(EflViewHostMsg_GstWebAudioDecode,
                    base::SharedMemoryHandle /* encoded_data_handle */,
                    base::FileDescriptor /* pcm_output */,
                    uint32_t /* data_size*/)
#endif
