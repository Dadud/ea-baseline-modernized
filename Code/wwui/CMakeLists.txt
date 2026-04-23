# Conservative scaffold for the original EA VC6 wwui static library.
#
# This target keeps the original project name and physical layout. The source
# groups below make dialog/control logic, input glue, IME integration, and UI
# presentation/render seams explicit before final architecture work.

set(WWUI_DIALOG_CONTROL_SOURCES
  ./ProgressCtrl.cpp
  ./ProgressCtrl.h
  ./buttonctrl.cpp
  ./buttonctrl.h
  ./checkboxctrl.cpp
  ./checkboxctrl.h
  ./childdialog.cpp
  ./childdialog.h
  ./comboboxctrl.cpp
  ./comboboxctrl.h
  ./controladvisesink.h
  ./dialogbase.cpp
  ./dialogbase.h
  ./dialogcontrol.cpp
  ./dialogcontrol.h
  ./dialogfactory.h
  ./dialogmgr.cpp
  ./dialogmgr.h
  ./dialogparser.cpp
  ./dialogparser.h
  ./dialogtext.cpp
  ./dialogtext.h
  ./dialogtransition.cpp
  ./dialogtransition.h
  ./dropdownctrl.cpp
  ./dropdownctrl.h
  ./editctrl.cpp
  ./editctrl.h
  ./healthbarctrl.cpp
  ./healthbarctrl.h
  ./imagectrl.cpp
  ./imagectrl.h
  ./inputctrl.cpp
  ./inputctrl.h
  ./listctrl.cpp
  ./listctrl.h
  ./listiconmgr.cpp
  ./listiconmgr.h
  ./mapctrl.cpp
  ./mapctrl.h
  ./menubackdrop.cpp
  ./menubackdrop.h
  ./menudialog.cpp
  ./menudialog.h
  ./menuentryctrl.cpp
  ./menuentryctrl.h
  ./merchandisectrl.cpp
  ./merchandisectrl.h
  ./multilinetextctrl.cpp
  ./multilinetextctrl.h
  ./popupdialog.cpp
  ./popupdialog.h
  ./scrollbarctrl.cpp
  ./scrollbarctrl.h
  ./shortcutbarctrl.cpp
  ./shortcutbarctrl.h
  ./sliderctrl.cpp
  ./sliderctrl.h
  ./stylemgr.cpp
  ./stylemgr.h
  ./tabctrl.cpp
  ./tabctrl.h
  ./textmarqueectrl.cpp
  ./textmarqueectrl.h
  ./tooltip.cpp
  ./tooltip.h
  ./tooltipmgr.cpp
  ./tooltipmgr.h
  ./treectrl.cpp
  ./treectrl.h
  ./viewerctrl.cpp
  ./viewerctrl.h
)

set(WWUI_INPUT_CURSOR_SOURCES
  ./mousemgr.cpp
  ./mousemgr.h
  ./screencursor.cpp
  ./screencursor.h
  ./wwuiinput.cpp
  ./wwuiinput.h
)

set(WWUI_IME_SOURCES
  ./IMECandidate.cpp
  ./IMECandidate.h
  ./IMECandidateCtrl.cpp
  ./IMECandidateCtrl.h
  ./IMEManager.cpp
  ./IMEManager.h
)

set(WWUI_SOURCES
  ${WWUI_DIALOG_CONTROL_SOURCES}
  ${WWUI_INPUT_CURSOR_SOURCES}
  ${WWUI_IME_SOURCES}
)

if(NOT WIN32)
  # wwui is heavily coupled to Win32 input/message-loop and IME APIs. Keep the
  # inventory visible, but defer the explicit Win32-facing input/IME islands in
  # the portable seam probe until a clearer UI/input/platform boundary exists.
  list(REMOVE_ITEM WWUI_SOURCES
    ${WWUI_INPUT_CURSOR_SOURCES}
    ${WWUI_IME_SOURCES}
  )
endif()

ea_vc6_static_library(wwui
  SOURCES
    ${WWUI_SOURCES}
  INCLUDE_DIRS
    ../DirectX/include
    ../miles6/include
    ../ww3d2
    ../wwaudio
    ../wwdebug
    ../wwlib
    ../WWMath
    ../wwsaveload
    ../wwtranslatedb
  DEFINES
    NDEBUG
    PARAM_EDITING_ON
    WIN32
    WWDEBUG
    _DEBUG
    _LIB
    _MBCS
    _WINDOWS
  LINK_LIBRARIES
    wwdebug
    wwlib
    wwmath
    wwsaveload
    wwtranslatedb
)
