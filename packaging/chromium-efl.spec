%if 0%{?nodebug}
%global __debug_install_post %{nil}
%global debug_package %{nil}
%endif

%if 0%{?_enable_unittests}
%define _debug_mode 1
%endif

Name: chromium-efl
Summary: Chromium EFL
# Set by by scripts/update-chromium-version.sh
%define ChromiumVersion 40.2214.38
%define Week 51
Version: %{ChromiumVersion}.%{Week}
Release: 1
Group: Applications/Internet
License: LGPLv2.1 or BSD

Source0: %{name}-%{version}.tar.gz
Source1: content_shell.in

Requires(post): /sbin/ldconfig
Requires(post): xkeyboard-config
Requires(postun): /sbin/ldconfig
BuildRequires: which, vi, python, python-xml, bison, flex, gperf, gettext, perl, edje-tools
BuildRequires: libjpeg-turbo-devel, expat-devel, libcap-devel
%if "%{?chromium_efl_tizen_version:%{chromium_efl_tizen_version}}%{!?chromium_efl_tizen_version:0}" == "2.4"
%ifarch %{arm}
BuildRequires: libhaptic
%endif
%endif
BuildRequires: pkgconfig(glesv2)
BuildRequires: pkgconfig(libpulse)
BuildRequires: pkgconfig(recordproto)
BuildRequires: pkgconfig(libpci)
BuildRequires: pkgconfig(pangocairo)
BuildRequires: pkgconfig(libudev)
BuildRequires: pkgconfig(fontconfig)
BuildRequires: pkgconfig(freetype2)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(harfbuzz)
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: pkgconfig(openssl)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(libxslt)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-system-sensor)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(capi-system-device)
BuildRequires: pkgconfig(capi-location-manager)
BuildRequires: pkgconfig(location)
BuildRequires: pkgconfig(gles20)
BuildRequires: pkgconfig(libpng12)
BuildRequires: pkgconfig(libusb-1.0)
BuildRequires: pkgconfig(speex)
BuildRequires: pkgconfig(flac)
BuildRequires: pkgconfig(minizip)
BuildRequires: pkgconfig(xrandr)
BuildRequires: pkgconfig(xcomposite)
BuildRequires: pkgconfig(xext)
BuildRequires: pkgconfig(xi)
BuildRequires: pkgconfig(xt)
BuildRequires: pkgconfig(xfixes)
BuildRequires: pkgconfig(xtst)
BuildRequires: pkgconfig(xdamage)
BuildRequires: pkgconfig(xcursor)
BuildRequires: pkgconfig(libdrm)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(ecore-x)
BuildRequires: pkgconfig(ecore-evas)
BuildRequires: pkgconfig(ecore-input)
BuildRequires: pkgconfig(ecore-imf-evas)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(ui-gadget-1)
BuildRequires: pkgconfig(libexif)
BuildRequires: pkgconfig(nspr)
BuildRequires: pkgconfig(zlib)
%if %{!?TIZEN_PROFILE_TV:1}%{?TIZEN_PROFILE_TV:0}
BuildRequires: bzip2-devel
BuildRequires: pkgconfig(efl-assist)
BuildRequires: pkgconfig(capi-network-connection)
%else # TIZEN_PROFILE_TV
BuildRequires: pkgconfig(audio-session-mgr)
BuildRequires: pkgconfig(mm-session)
%endif
BuildRequires: pkgconfig(gstreamer-0.10)
BuildRequires: pkgconfig(gstreamer-plugins-base-0.10)
BuildRequires: pkgconfig(gstreamer-app-0.10)
BuildRequires: pkgconfig(capi-media-camera)
BuildRequires: pkgconfig(capi-media-audio-io)
BuildRequires: pkgconfig(capi-media-player)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(libdri2)
BuildRequires: pkgconfig(libtbm)
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(feedback)
BuildRequires: pkgconfig(appcore-efl)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(ecore-imf)
BuildRequires: pkgconfig(scim)

%description
Browser Engine based on Chromium EFL (Shared Library)

%package devel
Summary: Chromium EFL
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
%description devel
Browser Engine dev library based on Chromium EFL (developement files)

%if 0%{?_enable_content_shell}
%package shell
Summary: Chromium EFL port of content_shell application
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
%description shell
Chromium EFL version of content_shell application
%endif

%if 0%{?_enable_unittests}
%package unittests
Summary: Chromium unittests
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
%description unittests
Chromium unite tests
%endif

%if 0%{?build_ewk_unittests}
%package ewktest
Summary: Chromium EWK unittests
Group: Development/UnitTests
Requires: %{name} = %{version}-%{release}
%description ewktest
Chromium EFL unit test utilities
%endif

# Directory for internal chromium executable components
%global CHROMIUM_EXE_DIR %{_libdir}/%{name}
# Constant read only data used by chromium-efl port
%global CHROMIUM_DATA_DIR %{_datadir}/%{name}
# Chromium unit tests install directory
%global CHROMIUM_UNITTESTS_DIR /opt/usr/chromium-unittests/
%global LOCALE_DIR /usr/share/chromium-efl

%prep
%setup -q

%build

# TODO : This execution will be removed by multimedia team
# Bug : http://107.108.218.239/bugzilla/show_bug.cgi?id=10516
%if %{!?chromium_efl_tizen_version:1}%{?chromium_efl_tizen_version:0}
build/apply_patches.sh build/patches
%endif

# workaround for new nss library : search it in /usr/lib first, rather than /lib (system nss)
export LD_RUN_PATH=%{_libdir}
#/usr/lib

# architecture related configuration + neon temporary workaround
%ifarch %{arm}
  export CFLAGS="$(echo $CFLAGS | sed 's/-mfpu=[a-zA-Z0-9-]*/-mfpu=neon/g')"
  export CXXFLAGS="$(echo $CXXFLAGS | sed 's/-mfpu=[a-zA-Z0-9-]*/-mfpu=neon/g')"
  export FFLAGS="$(echo $FFLAGS | sed 's/-mfpu=[a-zA-Z0-9-]*/-mfpu=neon/g')"
  %define EFL_TARGET arm
%else
  export CFLAGS="$(echo $CFLAGS | sed 's/-Wl,--as-needed//g')"
  export CXXFLAGS="$(echo $CXXFLAGS | sed 's/-Wl,--as-needed//g')"
  %if 0%{?simulator}
    %define EFL_TARGET emulator
  %else
    %define EFL_TARGET i386
  %endif
%endif

%if 0%{?nodebug}
  CFLAGS=$(echo $CFLAGS | sed 's/ -g / /')
  CXXFLAGS=$(echo $CXXFLAGS | sed 's/ -g / /')
%endif

%if %{!?TIZEN_PROFILE_TV:1}%{?TIZEN_PROFILE_TV:0}
  %define OUTPUT_BUILD_PROFILE_TARGET mobile
%else
  %define OUTPUT_BUILD_PROFILE_TARGET tv
%endif

%define OUTPUT_BASE_FOLDER out.%{OUTPUT_BUILD_PROFILE_TARGET}.%{EFL_TARGET}
export GYP_GENERATOR_OUTPUT=$PWD/%{OUTPUT_BASE_FOLDER}

#set build mode
%if 0%{?_debug_mode}
  %global OUTPUT_FOLDER %{OUTPUT_BASE_FOLDER}/Debug
%else
  %global OUTPUT_FOLDER %{OUTPUT_BASE_FOLDER}/Release
%endif

if type ccache &> /dev/null; then
  source build/ccache_env.sh %{OUTPUT_BUILD_PROFILE_TARGET}
fi

#gyp generate
%if %{?_skip_gyp:0}%{!?_skip_gyp:1}
#run standard gyp_chromiumefl wrapper
   ./build/gyp_chromiumefl.sh \
  -Dexe_dir="%{CHROMIUM_EXE_DIR}" \
  -Ddata_dir="%{CHROMIUM_DATA_DIR}" \
  -Dedje_dir="%{CHROMIUM_DATA_DIR}"/themes \
%if 0%{?_remove_webcore_debug_symbols:1}
  -Dremove_webcore_debug_symbols=1 \
%endif
%if 0%{?custom_libc_dir:1}
  -Dcustom_libc_dir=%{custom_libc_dir} \
%endif
%if 0%{?chromium_efl_tizen_version:1}
  -Dchromium_efl_tizen_version=%{chromium_efl_tizen_version} \
%endif
  -Dbuilding_for_tizen_"%{OUTPUT_BUILD_PROFILE_TARGET}"=1
%endif

%ifarch %{arm}
build/prebuild/ninja.arm %{_smp_mflags} -C"%{OUTPUT_FOLDER}" \
%else
build/prebuild/ninja %{_smp_mflags} -C"%{OUTPUT_FOLDER}" \
%endif
%if 0%{?_enable_content_shell}
  content_shell_efl \
%endif
%if 0%{?build_ewk_unittests}
  ewk_unittests \
%endif
  chromium-efl efl_webprocess chromium-ewk efl_webview_app mini_browser

%if 0%{?_enable_unittests}
ninja %{_smp_mflags} -C"%{OUTPUT_FOLDER}" angle_unittests env_chromium_unittests cacheinvalidation_unittests \
  url_unittests sandbox_linux_unittests crypto_unittests sql_unittests accessibility_unittests \
  gfx_unittests printing_unittests events_unittests ppapi_unittests jingle_unittests \
  flip_in_mem_edsm_server_unittests breakpad_unittests dbus_unittests libphonenumber_unittests \
  base_unittests ffmpeg_unittests gin_unittests net_unittests snapshot_unittests \
  google_apis_unittests
# TODO: Fix compilation of the following tests content_unittests cc_unittests shell_dialogs_unittests
# gpu_unittests compositor_unittests media_unittests
%endif

#XXX icudtl.dat is not copied by gyp. Do that manually
cp src/third_party/icu/android/icudtl.dat "%{OUTPUT_FOLDER}"

# TODO : This execution will be removed by multimedia team
# Bug : http://107.108.218.239/bugzilla/show_bug.cgi?id=10516
%if %{!?chromium_efl_tizen_version:1}%{?chromium_efl_tizen_version:0}
build/apply_patches.sh -r build/patches
%endif

%install

# Generate pkg-confg file
mkdir -p "%{OUTPUT_FOLDER}"/pkgconfig/
sed -e 's#?VERSION?#%{version}#' build/pkgconfig/chromium-efl.pc.in > "%{OUTPUT_FOLDER}"/pkgconfig/chromium-efl.pc

install -d "%{buildroot}"%{_sysconfdir}/smack/accesses2.d
install -d "%{buildroot}"%{_bindir}
install -d "%{buildroot}"%{_bindir}
install -d "%{buildroot}"%{_libdir}/pkgconfig
install -d "%{buildroot}"%{_includedir}/chromium-ewk
install -d "%{buildroot}"%{_includedir}/v8
install -d "%{buildroot}%{CHROMIUM_EXE_DIR}"
install -d "%{buildroot}%{CHROMIUM_DATA_DIR}"/themes
install -d "%{buildroot}%{CHROMIUM_DATA_DIR}"/locale

mkdir -p %{OUTPUT_FOLDER}/po_tizen
for file in ewk/po_tizen/*.po; do
  /usr/bin/msgfmt ${file} -o %{OUTPUT_FOLDER}/po_tizen/$(basename $file .po).mo
  install -d "%{buildroot}%{CHROMIUM_DATA_DIR}"/locale/$(basename $file .po)/LC_MESSAGES
  install -m 0755 %{OUTPUT_FOLDER}/po_tizen/$(basename $file .po).mo "%{buildroot}%{CHROMIUM_DATA_DIR}"/locale/$(basename $file .po)/LC_MESSAGES/WebKit.mo
done

install -m 0755 "%{OUTPUT_FOLDER}"/lib/libchromium-efl.so    "%{buildroot}"%{_libdir}
install -m 0755 "%{OUTPUT_FOLDER}"/lib/libchromium-ewk.so    "%{buildroot}"%{_libdir}

install -m 0755 "%{OUTPUT_FOLDER}"/libffmpegsumo.so  "%{buildroot}%{CHROMIUM_EXE_DIR}"
install -m 0755 "%{OUTPUT_FOLDER}"/efl_webprocess    "%{buildroot}%{CHROMIUM_EXE_DIR}"
install -m 0755 "%{OUTPUT_FOLDER}"/icudtl.dat    "%{buildroot}%{CHROMIUM_EXE_DIR}"
install -m 0755 "%{OUTPUT_FOLDER}"/content_shell.pak "%{buildroot}%{CHROMIUM_EXE_DIR}"
install -m 0644 "%{OUTPUT_FOLDER}"/resources/*.edj   "%{buildroot}%{CHROMIUM_DATA_DIR}"/themes

install -m 0755 "%{OUTPUT_FOLDER}"/efl_webview_app   "%{buildroot}"%{_bindir}
install -m 0755 "%{OUTPUT_FOLDER}"/mini_browser      "%{buildroot}"%{_bindir}

install -m 0644 "%{OUTPUT_FOLDER}"/pkgconfig/*.pc    "%{buildroot}"%{_libdir}/pkgconfig/
install -m 0644 ewk/efl_integration/public/*.h                  "%{buildroot}"%{_includedir}/chromium-ewk/
install -m 0644 src/v8/include/*.h "%{buildroot}"%{_includedir}/v8/

%if 0%{?_enable_content_shell}
install -m 0755 "%{OUTPUT_FOLDER}"/content_shell_efl "%{buildroot}%{CHROMIUM_EXE_DIR}"/content_shell
sed 's#@binary@#%{CHROMIUM_EXE_DIR}/content_shell#' %{SOURCE1} > "%{buildroot}"%{_bindir}/content_shell
%endif

install -d "%{buildroot}"/opt/share/packages
install -m 0644 ewk/efl_webview_app/chromium-efl.xml "%{buildroot}"/opt/share/packages

install -d "%{buildroot}"/opt/share/icons
install -m 0644 ewk/efl_webview_app/mini-browser.png "%{buildroot}"/opt/share/icons

%if 0%{?_enable_unittests}
install -d "%{INSTALL_ROOT}%{CHROMIUM_UNITTESTS_DIR}"
for test in "%{OUTPUT_FOLDER}/*_unittests"; do
  install -m 0755 ${test} "%{INSTALL_ROOT}%{CHROMIUM_UNITTESTS_DIR}"
done
%endif

%if 0%{?build_ewk_unittests}
mkdir -p %{buildroot}/opt/usr/resources/
mkdir -p %{buildroot}/opt/usr/utc_exec/
cp -r ewk/unittest/resources/* %{buildroot}/opt/usr/resources/
install -m 0644 "%{OUTPUT_FOLDER}"/ewk_unittests %{buildroot}/opt/usr/utc_exec/
install -m 0755 ewk/utc_gtest_run.sh %{buildroot}/opt/usr/utc_exec/
%endif


%post
# apply smack rule
smack_reload.sh

%postun

%files
%manifest packaging/%{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libchromium-efl.so
%{_libdir}/libchromium-ewk.so
%{CHROMIUM_EXE_DIR}/efl_webprocess
%{CHROMIUM_EXE_DIR}/libffmpegsumo.so
%{CHROMIUM_EXE_DIR}/icudtl.dat
%{CHROMIUM_EXE_DIR}/content_shell.pak
%{CHROMIUM_DATA_DIR}/themes/*.edj
%{_bindir}/efl_webview_app
%{_bindir}/mini_browser
/opt/share/packages/chromium-efl.xml
/opt/share/icons/mini-browser.png
%{CHROMIUM_DATA_DIR}/locale/*

%files devel
%defattr(-,root,root,-)
%{_includedir}/chromium-ewk/*.h
%{_libdir}/pkgconfig/*.pc
%{_includedir}/v8/*

%if 0%{?_enable_content_shell}
%files shell
%defattr(0755,root,root,-)
%{CHROMIUM_EXE_DIR}/content_shell
%{_bindir}/content_shell
%endif

%if 0%{?_enable_unittests}
%files unittests
%defattr(-,root,root,-)
%{CHROMIUM_UNITTESTS_DIR}/*
%endif

%if 0%{?build_ewk_unittests}
%files ewktest
%defattr(-,root,root,-)
%manifest ./packaging/chromium-ewktest.manifest
/opt/usr/utc_exec/*
/opt/usr/resources/*
%endif
