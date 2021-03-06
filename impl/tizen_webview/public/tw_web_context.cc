// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Copyright 2014-2015 Samsung Electronics. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "browser_context_efl.h"
#include "eweb_context.h"
#include "tw_web_context.h"

using content::BrowserContextEfl;

namespace tizen_webview {

namespace {
BrowserContextEfl::ResourceContextEfl* GetResourceContextEfl(
    EWebContext* ctx) {
  BrowserContextEfl::ResourceContextEfl* retval = NULL;

  if (ctx) {
    BrowserContextEfl* browser_context = ctx->browser_context();

    if (browser_context)
      retval = browser_context->GetResourceContextEfl();
  }
  return retval;
}
}

static WebContext* default_context_ = NULL;

// static
WebContext* WebContext::DefaultContext() {
  if (!default_context_) {
    // default context is not incognito
    default_context_ = new WebContext(false);
    default_context_->AddRef();
  }

  return default_context_;
}

// static
WebContext* WebContext::Create(bool incognito) {
  WebContext* context = new WebContext(incognito);
  context->AddRef();
  return context;
}

// static
void WebContext::Delete(WebContext* context) {
  if (context) {
    if (context == default_context_ && context->HasOneRef()) {
      // With chromium engine there is only single context
      // which is default context hence this delete
      // function will not be implemented
      NOTIMPLEMENTED();
      return;
    }
    context->Release();
  }
}

WebContext::WebContext(bool incognito) : impl(new EWebContext(incognito)) {
}

WebContext::~WebContext() {
  if (this == default_context_)
    default_context_= NULL;
  delete impl;
}

content::BrowserContextEfl* WebContext::browser_context() const {
  return impl->browser_context();
}

Ewk_Cookie_Manager* WebContext::ewkCookieManager() const {
  return impl->ewkCookieManager();
}

base::WeakPtr<CookieManager> WebContext::cookieManager() const {
  return impl->cookieManager();
}

void WebContext::SetDidStartDownloadCallback(Context_Did_Start_Download_Callback callback, void* user_data) {
  impl->SetDidStartDownloadCallback(callback, user_data);
}

void WebContext::SetCertificate(const char* certificate_file) {
  impl->SetCertificate(certificate_file);
}

void WebContext::SetProxyUri(const char* uri) {
  impl->SetProxyUri(uri);
}

const char* WebContext::GetProxyUri() const {
  return impl->GetProxyUri();
}

void WebContext::AddExtraPluginDir(const char *path) {
  impl->AddExtraPluginDir(path);
}

void WebContext::NotifyLowMemory() {
  impl->NotifyLowMemory();
}

bool WebContext::HTTPCustomHeaderAdd(const char* name, const char* value) {
  BrowserContextEfl::ResourceContextEfl* rc = GetResourceContextEfl(impl);

  if (rc)
    return rc->HTTPCustomHeaderAdd(name, value);

  return false;
}

bool WebContext::HTTPCustomHeaderRemove(const char* name) {
  BrowserContextEfl::ResourceContextEfl* rc = GetResourceContextEfl(impl);

  if (rc)
    return rc->HTTPCustomHeaderRemove(name);

  return false;
}

void WebContext::HTTPCustomHeaderClear() {
  BrowserContextEfl::ResourceContextEfl* rc = GetResourceContextEfl(impl);

  if (rc)
    rc->HTTPCustomHeaderClear();
}

void WebContext::SetCacheModel(tizen_webview::Cache_Model cm) {
  impl->SetCacheModel(cm);
}

tizen_webview::Cache_Model WebContext::GetCacheModel() const {
  return impl->GetCacheModel();
}

void WebContext::SetNetworkCacheEnable(bool enable) {
  impl->SetNetworkCacheEnable(enable);
}

bool WebContext::GetNetworkCacheEnable() const {
  return impl->GetNetworkCacheEnable();
}

void WebContext::ClearNetworkCache() {
  impl->ClearNetworkCache();
}

void WebContext::ClearWebkitCache() {
  impl->ClearWebkitCache();
}

void WebContext::GetAllOriginsWithApplicationCache(Web_Application_Cache_Origins_Get_Callback callback, void* user_data) {
  impl->GetAllOriginsWithApplicationCache(callback, user_data);
}

void WebContext::GetApplicationCacheUsage(const URL& url, Web_Application_Cache_Usage_For_Origin_Get_Callback callback, void* user_data) {
  impl->GetApplicationCacheUsage(url, callback, user_data);
}

void WebContext::DeleteAllApplicationCache() {
  impl->DeleteAllApplicationCache();
}

void WebContext::DeleteApplicationCacheForSite(const tizen_webview::URL& url) {
  impl->DeleteApplicationCacheForSite(url);
}

void WebContext::GetAllOriginsWithWebDB(Web_Database_Origins_Get_Callback callback, void* user_data) {
  impl->GetAllOriginsWithWebDB(callback, user_data);
}

void WebContext::WebDBDelete(const URL& host) {
  impl->WebDBDelete(host);
}

void WebContext::IndexedDBDelete() {
  impl->IndexedDBDelete();
}

void WebContext::WebStorageOriginsAllGet(Web_Storage_Origins_Get_Callback callback, void* user_data) {
  impl->WebStorageOriginsAllGet(callback, user_data);
}

void WebContext::WebStorageDelete() {
  impl->WebStorageDelete();
}

void WebContext::WebStorageDelete(const URL& origin) {
  impl->WebStorageDelete(origin);
}

void WebContext::GetAllOriginsWithFileSystem(Local_File_System_Origins_Get_Callback callback, void* user_data) const {
  impl->GetAllOriginsWithFileSystem(callback, user_data);
}

void WebContext::FileSystemDelete(const URL& host) {
  impl->FileSystemDelete(host);
}

bool WebContext::SetFaviconDatabasePath(const char *path) {
  return impl->SetFaviconDatabasePath(path);
}

#if defined(OS_TIZEN_TV)
FaviconDatabase* WebContext::GetFaviconDatabase() {
  return impl->GetFaviconDatabase();
}
#endif

Evas_Object * WebContext::AddFaviconObject(const char *uri, Evas *canvas) const {
  return impl->AddFaviconObject(uri, canvas);
}

void WebContext::SendWidgetInfo(int widget_id, double scale, const std::string &theme, const std::string &encoded_bundle) {
  impl->SendWidgetInfo(widget_id, scale, theme, encoded_bundle);
}

int WebContext::Pixmap() const {
  return impl->Pixmap();
}

void WebContext::SetPixmap(int pixmap) {
  impl->SetPixmap( pixmap);
}

void WebContext::SetMimeOverrideCallback(Mime_Override_Callback callback) {
  EWebContext::SetMimeOverrideCallback(callback);
}

void WebContext::ClearCandidateData() {
  impl->ClearCandidateData();
}

void WebContext::ClearPasswordData() {
  impl->ClearPasswordData();
}

unsigned int WebContext::InspectorServerStart(unsigned int port) const {
  return impl->InspectorServerStart(port);
}

bool WebContext::InspectorServerStop() const {
  return impl->InspectorServerStop();
}

} // namespace tizen_webview
