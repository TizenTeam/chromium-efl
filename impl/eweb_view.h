/*
    Copyright (C) 2014 Samsung Electronics

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef EWEB_VIEW_H
#define EWEB_VIEW_H

#if defined(OS_TIZEN_MOBILE)
#include <appfw/app_service.h>
#include <vector>
#endif

#include <map>
#include <string>
#include <Evas.h>
#include <locale.h>

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "content/public/common/context_menu_params.h"
#include "content/public/common/file_chooser_params.h"
#include "content/browser/renderer_host/event_with_latency_info.h"
#include "content/common/input/input_event_ack_state.h"
#include "content/public/common/menu_item.h"
#include "base/id_map.h"
#include "context_menu_controller_efl.h"
#include "eweb_context.h"
#include "API/ewk_hit_test_private.h"
#include "API/ewk_auth_challenge_private.h"
#include "API/ewk_settings_private.h"
#include "API/ewk_web_application_icon_data_private.h"
#include "eweb_view_callbacks.h"
#include "public/ewk_view.h"
#include "selection_controller_efl.h"
#include "web_contents_delegate_efl.h"
#include "ui/events/gestures/gesture_types.h"
#include "context_menu_controller_efl.h"
#include "file_chooser_controller_efl.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"
#include "browser/inputpicker/InputPicker.h"
#include "third_party/WebKit/Source/platform/text/TextDirection.h"
#include "tizen_webview/public/tw_hit_test.h"

#ifdef TIZEN_CONTENTS_DETECTION
#include "popup_controller_efl.h"
#endif

#ifdef TIZEN_EDGE_EFFECT
#include "EdgeEffect.h"
#endif

#if defined(OS_TIZEN)
#include "browser/selectpicker/popup_picker.h"
#endif

#include "tizen_webview/public/tw_hit_test.h"
#include "tizen_webview/public/tw_touch_event.h"
#include "tizen_webview/public/tw_callbacks.h"
#include "tizen_webview/public/tw_content_security_policy.h"
#include "tizen_webview/public/tw_input_type.h"
#include "tizen_webview/public/tw_find_options.h"
#include "tizen_webview/public/tw_touch_event.h"

namespace content {
class RenderViewHost;
class RenderWidgetHostViewEfl;
class WebContentsDelegateEfl;
class ContextMenuControllerEfl;
class DevToolsDelegateEfl;
#ifdef TIZEN_CONTENTS_DETECTION
class PopupControllerEfl;
#endif
}

namespace ui {
class GestureRecognizer;
class GestureEvent;
class TouchEvent;
}

namespace tizen_webview {
class Hit_Test;
class WebContext;
class WebView;
class PolicyDecision;
}

class EwkViewPlainTextGetCallback {
 public:
  EwkViewPlainTextGetCallback(tizen_webview::View_Plain_Text_Get_Callback callback,
                              void* user_data)
    : callback_(callback), user_data_(user_data)
    { }
  void TriggerCallback(Evas_Object* obj, const std::string& content_text);

 private:
  tizen_webview::View_Plain_Text_Get_Callback callback_;
  void* user_data_;
};

class OrientationLockCallback {
 public:
  OrientationLockCallback(tizen_webview::Orientation_Lock_Cb lock,
                          void* user_data)
    : lock_(lock),
      user_data_(user_data)
    {}
  private:
    tizen_webview::Orientation_Lock_Cb lock_;
    void* user_data_;
};

class MHTMLCallbackDetails {
 public:
  MHTMLCallbackDetails(tizen_webview::View_MHTML_Data_Get_Callback callback_func, void *user_data)
    : callback_func_(callback_func),
      user_data_(user_data)
  {}
  void Run(Evas_Object* obj, const std::string& mhtml_content);

  tizen_webview::View_MHTML_Data_Get_Callback callback_func_;
  void *user_data_;
};

class WebApplicationIconUrlGetCallback {
 public:
  WebApplicationIconUrlGetCallback(tizen_webview::Web_App_Icon_URL_Get_Callback func, void *user_data)
    : func_(func), user_data_(user_data)
  {}
  void Run(const std::string &url) {
    if (func_) {
      (func_)(url.c_str(), user_data_);
    }
  }

 private:
  tizen_webview::Web_App_Icon_URL_Get_Callback func_;
  void *user_data_;
};

class WebApplicationIconUrlsGetCallback {
 public:
  WebApplicationIconUrlsGetCallback(tizen_webview::Web_App_Icon_URLs_Get_Callback func, void *user_data)
    : func_(func), user_data_(user_data)
  {}
  void Run(const std::map<std::string, std::string> &urls) {
    if (func_) {
      Eina_List *list = NULL;
      for (std::map<std::string, std::string>::const_iterator it = urls.begin(); it != urls.end(); ++it) {
        _Ewk_Web_App_Icon_Data *data = ewkWebAppIconDataCreate(it->first, it->second);
        list = eina_list_append(list, data);
      }
      (func_)(list, user_data_);
    }
  }

 private:
  tizen_webview::Web_App_Icon_URLs_Get_Callback func_;
  void *user_data_;
};

class WebApplicationCapableGetCallback {
 public:
  WebApplicationCapableGetCallback(tizen_webview::Web_App_Capable_Get_Callback func, void *user_data)
    : func_(func), user_data_(user_data)
  {}
  void Run(bool capable) {
    if (func_) {
      (func_)(capable ? EINA_TRUE : EINA_FALSE, user_data_);
    }
  }

 private:
  tizen_webview::Web_App_Capable_Get_Callback func_;
  void *user_data_;
};

class JavaScriptDialogManagerEfl;
class WebViewBrowserMessageFilter;

class EWebView
    : public ui::GestureConsumer
    , public ui::GestureEventHelper {
 public:
  static bool InitSmartClassInterface(Ewk_View_Smart_Class&);

  static EWebView* Create(tizen_webview::WebView* owner,
    tizen_webview::WebContext* context, Evas* canvas, Evas_Smart* smart = 0);

  void CreateNewWindow(content::WebContents* new_contents);

  tizen_webview::WebContext* context() const { return context_; }
  Evas_Object* evas_object() const { return evas_object_; }

  Evas* GetEvas() const { return evas_object_evas_get(evas_object_); }
  Evas_Object* GetContentImageObject() const;

  content::WebContentsDelegateEfl* web_contents_delegate() const
  {
    return web_contents_delegate_.get();
  }

  template<EWebViewCallbacks::CallbackType callbackType>
  EWebViewCallbacks::CallBack<callbackType> SmartCallback() const
  {
    return EWebViewCallbacks::CallBack<callbackType>(evas_object_);
  }

  void set_magnifier(bool status);

  // ewk_view api
  void SetURL(const char* url_string);
  const char* GetURL() const;
  void Reload();
  void ReloadIgnoringCache();
  Eina_Bool CanGoBack();
  Eina_Bool CanGoForward();
  Eina_Bool HasFocus() const;
  void SetFocus(Eina_Bool focus);
  Eina_Bool GoBack();
  Eina_Bool GoForward();
  void Suspend();
  void Resume();
  void Stop();
  double GetTextZoomFactor() const;
  void SetTextZoomFactor(double text_zoom_factor);
  void ExecuteEditCommand(const char* command, const char* value);
  void SendOrientationChangeEventIfNeeded(int orientation);
  void SetOrientationLockCallback(tizen_webview::Orientation_Lock_Cb func, void* data);
  bool TouchEventsEnabled() const;
  void SetTouchEventsEnabled(bool enabled);
  bool MouseEventsEnabled() const;
  void SetMouseEventsEnabled(bool enabled);
  void HandleTouchEvents(tizen_webview::Touch_Event_Type type, const Eina_List *points, const Evas_Modifier *modifiers);
  void Show();
  void Hide();
  bool ExecuteJavaScript(const char* script, tizen_webview::View_Script_Execute_Callback callback, void* userdata);
  bool SetUserAgent(const char* userAgent);
  bool SetUserAgentAppName(const char* application_name);
  const char* GetUserAgent() const;
  const char* GetUserAgentAppName() const;
  const char* GetSelectedText() const;
  Ewk_Settings* GetSettings();
  void UpdateWebKitPreferences();
  void LoadHTMLString(const char* html, const char* base_uri, const char* unreachable_uri);
  void LoadPlainTextString(const char* plain_text);
  void LoadData(const char* data, size_t size, const char* mime_type, const char* encoding, const char* base_uri, const char* unreachable_uri = NULL);
  void InvokeAuthCallback(LoginDelegateEfl* login_delegate, const GURL& url, const std::string& realm);
  void Find(const char* text, tizen_webview::Find_Options);
  void InvokeAuthCallbackOnUI(_Ewk_Auth_Challenge* auth_challenge);
  void SetContentSecurityPolicy(const char* policy, tizen_webview::ContentSecurityPolicyType type);
  void ShowPopupMenu(const gfx::Rect& rect, blink::TextDirection textDirection, double pageScaleFactor, const std::vector<content::MenuItem>& items, int data, int selectedIndex, bool multiple);
  Eina_Bool HidePopupMenu();
  bool FormIsNavigating() const { return formIsNavigating_; }
  void SetFormIsNavigating(bool formIsNavigating);
  Eina_Bool PopupMenuUpdate(Eina_List* items, int selectedIndex);
  Eina_Bool DidSelectPopupMenuItem(int selectedindex);
  Eina_Bool DidMultipleSelectPopupMenuItem(std::vector<int>& selectedindex);
  Eina_Bool PopupMenuClose();
  void ShowContextMenu(const content::ContextMenuParams& params, content::ContextMenuType type = content::MENU_TYPE_LINK);
  void CancelContextMenu(int request_id);
  void SetScale(double scale_factor, int x, int y);
  void GetScrollPosition(int* x, int* y) const;
  void SetScroll(int x, int y);
  void UrlRequestSet(const char* url, std::string method, Eina_Hash* headers, const char* body);

  content::SelectionControllerEfl* GetSelectionController() const { return selection_controller_.get(); }
#ifdef TIZEN_CONTENTS_DETECTION
  content::PopupControllerEfl* GetPopupController() const { return popup_controller_.get(); }
#endif
  void SelectRange(const gfx::Point& start, const gfx::Point& end);
  void MoveCaret(const gfx::Point& point);
  void QuerySelectionStyle();
  void OnQuerySelectionStyleReply(const SelectionStylePrams& params);
  void SelectClosestWord(const gfx::Point& touch_point);
  void SelectLinkText(const gfx::Point& touch_point);
  bool GetSelectionRange(Eina_Rectangle* left_rect, Eina_Rectangle* right_rect);
  bool ClearSelection();
  void GetSnapShotForRect(gfx::Rect& rect);

  // Callback OnCopyFromBackingStore will be called once we get the snapshot from render
  void OnCopyFromBackingStore(bool success, const SkBitmap& bitmap);

  /**
   * Creates a snapshot of given rectangle from EWebView
   *
   * @param rect rectangle of EWebView which will be taken into snapshot
   *
   * @return created snapshot or NULL if error occured.
   * @note ownership of snapshot is passed to caller
  */
  Evas_Object* GetSnapshot(Eina_Rectangle rect);
  void set_policy_decision(tizen_webview::PolicyDecision* pr) { policy_decision_.reset(pr); }
  tizen_webview::PolicyDecision* get_policy_decision() const { return policy_decision_.get(); }
  void InvokePolicyResponseCallback(tizen_webview::PolicyDecision* policy_decision);
  void InvokePolicyNavigationCallback(content::RenderViewHost* rvh,
      NavigationPolicyParams params, bool* handled);
  void UseSettingsFont();

  tizen_webview::Hit_Test* RequestHitTestDataAt(int x, int y, tizen_webview::Hit_Test_Mode mode);
  tizen_webview::Hit_Test* RequestHitTestDataAtBlinkCoords(int x, int y, tizen_webview::Hit_Test_Mode mode);
  void UpdateHitTestData(const _Ewk_Hit_Test& hit_test_data, const NodeAttributesMap& node_attributes);

  int current_find_request_id() const { return current_find_request_id_; }
  bool PlainTextGet(tizen_webview::View_Plain_Text_Get_Callback callback, void* user_data);
  void InvokePlainTextGetCallback(const std::string& content_text, int plain_text_get_callback_id);
  int SetEwkViewPlainTextGetCallback(tizen_webview::View_Plain_Text_Get_Callback callback, void* user_data);
  void DidChangeContentsSize(int width, int height);
  const Eina_Rectangle GetContentsSize() const;
  void GetScrollSize(int* w, int* h);
  void StopFinding();
  void SetProgressValue(double progress);
  double GetProgressValue();
  const char* GetTitle();
  bool SaveAsPdf(int width, int height, const std::string& file_name);
  void BackForwardListClear();
  void InvokeBackForwardListChangedCallback();
  bool WebAppCapableGet(tizen_webview::Web_App_Capable_Get_Callback callback, void *userData);
  bool WebAppIconUrlGet(tizen_webview::Web_App_Icon_URL_Get_Callback callback, void *userData);
  bool WebAppIconUrlsGet(tizen_webview::Web_App_Icon_URLs_Get_Callback callback, void *userData);
  void InvokeWebAppCapableGetCallback(bool capable, int callbackId);
  void InvokeWebAppIconUrlGetCallback(const std::string &iconUrl, int callbackId);
  void InvokeWebAppIconUrlsGetCallback(const std::map<std::string, std::string> &iconUrls, int callbackId);

  bool GetMHTMLData(tizen_webview::View_MHTML_Data_Get_Callback callback, void* user_data);
  void OnMHTMLContentGet(const std::string& mhtml_content, int callback_id);
  bool IsFullscreen();
  void ExitFullscreen();
  double GetScale();
  void DidChangePageScaleFactor(double scale_factor);
  void SetJavaScriptAlertCallback(tizen_webview::View_JavaScript_Alert_Callback callback, void* user_data);
  void JavaScriptAlertReply();
  void SetJavaScriptConfirmCallback(tizen_webview::View_JavaScript_Confirm_Callback callback, void* user_data);
  void JavaScriptConfirmReply(bool result);
  void SetJavaScriptPromptCallback(tizen_webview::View_JavaScript_Prompt_Callback callback, void* user_data);
  void JavaScriptPromptReply(const char* result);
  void set_renderer_crashed();
  void GetPageScaleRange(double *min_scale, double *max_scale);
  void DidChangePageScaleRange(double min_scale, double max_scale);
  void SetDrawsTransparentBackground(bool enabled);
  void GetSessionData(const char **data, unsigned *length) const;
  bool RestoreFromSessionData(const char *data, unsigned length);
  void ProcessAckedTouchEvent(const content::TouchEventWithLatencyInfo& touch,
                              content::InputEventAckState ack_result);
  void ShowFileChooser(const content::FileChooserParams&);
  void DidChangeContentsArea(int width, int height);
  void SetBrowserFont();

  void RequestColorPicker(int r, int g, int b, int a);
  void DismissColorPicker();
  bool SetColorPickerColor(int r, int g, int b, int a);
  void InputPickerShow(tizen_webview::Input_Type inputType, const char* inputValue);

#ifdef TIZEN_EDGE_EFFECT
  void SetSettingsGetCallback(Ewk_View_Settings_Get callback, void* user_data);
  scoped_refptr<EdgeEffect> edgeEffect() { return edge_effect_; }
#endif
#ifdef TIZEN_CONTENTS_DETECTION
  void ShowContentsDetectedPopup(const char*);
#endif

  bool IsIMEShow();
  gfx::Rect GetIMERect();

  // Returns TCP port number with Inspector, or 0 if error.
  int StartInspectorServer(int port = 0);
  bool StopInspectorServer();

  std::string GetErrorPage(const std::string& invalidUrl);
  static std::string GetPlatformLocale();
  void UpdateMagnifierScreen(const SkBitmap& bitmap);
  bool GetLinkMagnifierEnabled() const;
  void SetLinkMagnifierEnabled(bool enabled);

  void SetOverrideEncoding(const std::string& encoding);

 private:
  EWebView(tizen_webview::WebContext*, Evas_Object* smart_object);
  ~EWebView();

  // Evas_Smart_Class callback interface:
  static void handleEvasObjectAdd(Evas_Object*);
  static void handleEvasObjectDelete(Evas_Object*);
  static void handleEvasObjectShow(Evas_Object*);
  static void handleEvasObjectHide(Evas_Object*);
  static void handleEvasObjectMove(Evas_Object*, Evas_Coord x, Evas_Coord y);
  static void handleEvasObjectResize(Evas_Object*, Evas_Coord width, Evas_Coord height);
  static void handleEvasObjectCalculate(Evas_Object*);
  static void handleEvasObjectColorSet(Evas_Object*, int red, int green, int blue, int alpha);

  // Ewk_View_Smart_Class callback interface:
  static Eina_Bool handleFocusIn(Ewk_View_Smart_Data* d);
  static Eina_Bool handleFocusOut(Ewk_View_Smart_Data* d);
  static Eina_Bool handleMouseWheel(Ewk_View_Smart_Data* d, const Evas_Event_Mouse_Wheel* wheelEvent);
  static Eina_Bool handleMouseDown(Ewk_View_Smart_Data* d, const Evas_Event_Mouse_Down* downEvent);
  static Eina_Bool handleMouseUp(Ewk_View_Smart_Data* d, const Evas_Event_Mouse_Up* upEvent);
  static Eina_Bool handleMouseMove(Ewk_View_Smart_Data* d, const Evas_Event_Mouse_Move* moveEvent);
  static Eina_Bool handleKeyDown(Ewk_View_Smart_Data* d, const Evas_Event_Key_Down* downEvent);
  static Eina_Bool handleKeyUp(Ewk_View_Smart_Data* d, const Evas_Event_Key_Up* upEvent);
  static Eina_Bool handleTextSelectionDown(Ewk_View_Smart_Data* d, int x, int y);
  static Eina_Bool handleTextSelectionUp(Ewk_View_Smart_Data* d, int x, int y);

  static unsigned long long handleExceededDatabaseQuota(Ewk_View_Smart_Data *sd, const char *databaseName, const char *displayName, unsigned long long currentQuota, unsigned long long currentOriginUsage, unsigned long long currentDatabaseUsage, unsigned long long expectedUsage);

  static void OnTouchDown(void*, Evas*, Evas_Object*, void*);
  static void OnTouchUp(void*, Evas*, Evas_Object*, void*);
  static void OnTouchMove(void*, Evas*, Evas_Object*, void*);
#if defined(OS_TIZEN_MOBILE)
  static void cameraResultCb(service_h request, service_h reply,
    service_result_e result, void* data);
#endif

  void HandleTouchEvents(tizen_webview::Touch_Event_Type);

  void SubscribeMouseEvents();
  void UnsubscribeMouseEvents();
  void SubscribeTouchEvents();
  void UnsubscribeTouchEvents();
#ifdef OS_TIZEN
  void SubscribeMotionEvents();
  void UnsubscribeMotionEvents();
  bool GetTiltZoomEnabled();

  static void OnMotionZoom(void *data, Evas_Object *obj, void *eventInfo);
  static void OnMotionEnable(void *data, Evas_Object *obj, void *eventInfo);
  static void OnMotionMove(void *data, Evas_Object *obj, void *eventInfo);
#endif

  // GestureEventHelper overrides
  virtual bool CanDispatchToConsumer(ui::GestureConsumer* consumer) OVERRIDE;
  virtual void DispatchPostponedGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void DispatchCancelTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void DispatchGestureEvent(ui::GestureEvent*) OVERRIDE;

#if defined(OS_TIZEN_MOBILE)
  bool LaunchCamera(base::string16 mimetype);
#endif
  content::RenderWidgetHostViewEfl* rwhv() const;
  JavaScriptDialogManagerEfl* GetJavaScriptDialogManagerEfl();

  static Evas_Smart_Class parent_smart_class_;

  // For popup windows the WebContents is created internally and we need to associate it with the
  // new view created by the embedder. We set this before calling the "create,window" callback and
  // use it for the new view. This is a hack! It would break if the first view the embedder creates
  // in the callback is not the result of the callback. Unfortunately this does not seems to be solvable
  // with the current design of the content API.
  // FIXME: should we add documentation for that?
  static content::WebContents* contents_for_new_window_;

  scoped_refptr<tizen_webview::WebContext> context_;
  scoped_ptr<content::WebContentsDelegateEfl> web_contents_delegate_;
  std::string pending_url_request_;
  scoped_ptr<Ewk_Settings> settings_;
  scoped_ptr<tizen_webview::PolicyDecision> window_policy_;
  Evas_Object* evas_object_;
  bool touch_events_enabled_;
  bool mouse_events_enabled_;
  scoped_ptr<ui::GestureRecognizer> gesture_recognizer_;
  double text_zoom_factor_;
  std::string overridden_user_agent_;
  mutable std::string selected_text_;
  scoped_ptr<_Ewk_Auth_Challenge> auth_challenge_;
  scoped_ptr<tizen_webview::PolicyDecision> policy_decision_;
#if defined(OS_TIZEN)
  Eina_List* popupMenuItems_;
  Popup_Picker* popupPicker_;
#endif
  bool formIsNavigating_;
  struct {
    int count;
    int position;
    bool prevState;
    bool nextState;
  } formNavigation;
  scoped_ptr<content::ContextMenuControllerEfl> context_menu_;
  scoped_ptr<content::FileChooserControllerEfl> file_chooser_;
#ifdef TIZEN_CONTENTS_DETECTION
  scoped_ptr<content::PopupControllerEfl> popup_controller_;
#endif
  scoped_ptr<content::SelectionControllerEfl> selection_controller_;
  base::string16 previous_text_;
  int current_find_request_id_;
  static int find_request_id_counter_;
  IDMap<EwkViewPlainTextGetCallback, IDMapOwnPointer> plain_text_get_callback_map_;
  gfx::Size contents_size_;
  gfx::Size contents_area_;
  double progress_;
  mutable std::string title_;
  _Ewk_Hit_Test hit_test_data_;
  base::WaitableEvent hit_test_completion_;
  IDMap<MHTMLCallbackDetails, IDMapOwnPointer> mhtml_callback_map_;
  double page_scale_factor_;
  double min_page_scale_factor_;
  double max_page_scale_factor_;
  scoped_ptr<OrientationLockCallback> orientation_lock_callback_;
  scoped_ptr<content::InputPicker> inputPicker_;
  IDMap<WebApplicationIconUrlGetCallback, IDMapOwnPointer> web_app_icon_url_get_callback_map_;
  IDMap<WebApplicationIconUrlsGetCallback, IDMapOwnPointer> web_app_icon_urls_get_callback_map_;
  IDMap<WebApplicationCapableGetCallback, IDMapOwnPointer> web_app_capable_get_callback_map_;
  content::DevToolsDelegateEfl* inspector_server_;
#ifdef TIZEN_EDGE_EFFECT
  scoped_refptr<EdgeEffect> edge_effect_;
#endif
  WebViewBrowserMessageFilter* message_filter_;

#ifndef NDEBUG
  bool renderer_crashed_;
#endif
#if defined(OS_TIZEN_MOBILE)
  content::FileChooserParams::Mode filechooser_mode_;
#endif

  // should not used outside of WebView
  friend class tizen_webview::WebView;
  static void Delete(EWebView* ewv);
  void SetPublicWebView(tizen_webview::WebView* wv);
  tizen_webview::WebView* GetPublicWebView();
  tizen_webview::WebView* public_webview_;
};

bool IsEWebViewObject(const Evas_Object*);
EWebView* ToEWebView(const Evas_Object*);

#endif
