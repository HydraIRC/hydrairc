;NSIS installer script for HydraIRC
;for NSIS version 2.03
;Modern User Interface version 1.67
;© Rovastar and Zann for Hydra Productions
;Revision 0.5, December 2003
;
;ToDo:
; - Can't think of anything else atm, maybe custom incons for (un-)*.exe's ?
; - Suggestions are very welcome.
;
;Revision 0.7  February 2004
; Hydra
; - Only files matching "plugin_*.dll" & "plugin.dll" are deleted from
;   the .\plugins\ folder
;
;Revision 0.6  February 2004
; Zann
; - Adjusted for NSIS v2.0 Final
; - Only files matching "plugin*.dll" are deleted from the .\plugins\ folder
;
;Revision 0.5  December 2003
; Zann
; - LZMA is now used for compression (new in NSIS v2.0b5 CVS)
; - Added custom graphics by digismack (thanks, man!) to the installer GUI
; - Fixed misleading "Copy failed" log message on very first installation
; - States of check boxes on the FINISH page are now remembered and re-used
;   (at last!) via Pre- and Leave- FINISH page custom functions
; - Re-wrote and merged install components registry reading code from .onInit
;   with *.xml presence checking code from .onVerifyInstDir, and moved it all
;   to custom Pre- function for COMPONENTS page
; - Removed <Cancel> button on FINISH page (requested by johnp)
; - Added lotsa fun stuff to HKLM\...\Uninstall in the registry. Try clicking
;   "Support Information" in the Control Panel Add/Remove Programs applet
; - Adapted to *massive* changes in the NSIS v2.0b4 Modern UI system
; - Added WELCOME and FINISH pages to the un-installer (new in NSIS v2.0b4)
;   another chance to flash digismack's art too :)
; - "View Readme" renamed to "View Latest Changes" (new in NSIS v2.0b4)
; - Added link to the web site on the finish page (new in NSIS v2.0b4)
; - Branding text now uses ${NSIS_VERSION} variable
;
;Revision 0.4  June 2003
; Zann
; - Updated to NSIS v2.0b4 CVS
; - Directory page is shown before the options page
; - Enables/disables Use Previous Settings option depending on $INSTDIR
;
;Revision 0.3  June 2003
; Zann
; - When installing in the same dir, deletes all files from ..\plugins\ folder
; - When uninstalling, prompts user to acknowledge that everything will be deleted
; - Changed branding text - couldn't see the problem though :)
; - When doing "light" uninstall, user can still use add/remove programs to uninstall the files
;
;Revision 0.2  May 2003
; Zann
; - Updated to NSIS v2.0b3
; - Remembers previously used installation directory
; - Remembers what optional components were previously checked
; - When preserving settings, only two *.xml files are fiddled with
; - Keeps both new and old xml files available, but makes either active depending on user's choice
; - Quick Launch shortcut option
; - Uninstaller: a choice of complete removal, to keep files and folders but remove all shortcuts and registry settings, or to cancel
;
;Revision 0.1
; Rovastar
; - First functional version
;


;Here we go! Installer script starts here :)
!define PRODUCT "HydraIRC"
!define VERSION "0.3.164"
Name "${PRODUCT}"

;Installer will be compiled into. Don't forget to update VERSION above!
OutFile ..\Distribution\${PRODUCT}-v${VERSION}-Release.exe
;OutFile ..\Distribution\${PRODUCT}-v${VERSION}-TestXP.exe
;OutFile ..\Distribution\${PRODUCT}-v${VERSION}-Debug.exe

;<start> Installer Configuration
;-------------------------------
;SetCompressor lzma ;Compression to use
ShowInstDetails show ;Shows what's being done
ShowUninstDetails show
SetOverwrite on
SetDateSave on
BrandingText /TRIMRIGHT "${PRODUCT} v${VERSION}"
InstallDir $PROGRAMFILES\${PRODUCT} ;Default installation folder
;The following makes the installer to look for InstallDir in the registry
;and use what was found - otherwise it will use InstallDir defined above
InstallDirRegKey HKLM "SOFTWARE\HydraProductions\${PRODUCT}" "InstallPath"
;-------------------------------
;<end> Installer Configuration


;<start> Modern UI Configuration
;-------------------------------
!include "MUI.nsh"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "welcome.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcome.bmp"
!define MUI_ABORTWARNING ;confirmation on cancel
!define MUI_COMPONENTSPAGE_SMALLDESC ;makes Descriptions go under Options
!define MUI_FINISHPAGE_NOAUTOCLOSE ;prevents from jumping to Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\hydrairc.exe" ;checkbox to run the prog
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\changes.txt" ;checkbox to see a file
!define MUI_FINISHPAGE_SHOWREADME_TEXT "View Latest Changes"
!define MUI_FINISHPAGE_LINK "Visit ${PRODUCT} Web-Site"
!define MUI_FINISHPAGE_LINK_LOCATION "http://www.hydrairc.com"
!define MUI_FINISHPAGE_LINK_COLOR 127ED6
!define MUI_FINISHPAGE_NOREBOOTSUPPORT ;saves room by omitting reboot code on compile

!insertmacro MUI_PAGE_WELCOME ;includes Welcome page - remove?
!insertmacro MUI_PAGE_LICENSE "..\Distribution\Release\license.txt" ;includes Licence page
!insertmacro MUI_PAGE_DIRECTORY ;includes Choose Install Directory page

!define MUI_PAGE_CUSTOMFUNCTION_PRE myPreComponentsFunction
!insertmacro MUI_PAGE_COMPONENTS ;includes Install Options page
!insertmacro MUI_PAGE_INSTFILES ;includes progress page (log, progress bar)

!define MUI_PAGE_CUSTOMFUNCTION_PRE myPreFinishFunction
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE myLeaveFinishFunction
!insertmacro MUI_PAGE_FINISH ;includes Finish Page

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM ;includes un-installer page 1
!insertmacro MUI_UNPAGE_INSTFILES ;includes un-installer page 2
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "English" ;Language to use
;-------------------------------
;<end> Modern UI Configuration


;<start> Sections (Install Options)
;-------------------------------
Section "${PRODUCT} Main Program Files (Required)" SecCore

  SectionIn RO ;Makes the section read-only

  SetOutPath $INSTDIR

  ;Keep current .xml as .old and delete.new if any
  IfFileExists $INSTDIR\Profile_Default.xml ""
  Rename $INSTDIR\Profile_Default.xml $INSTDIR\Profile_Default.old
  IfFileExists $INSTDIR\Profile_Default.new ""
  Delete $INSTDIR\Profile_Default.new

  ;Keep current .xml as .old and delete.new if any
  IfFileExists $INSTDIR\HydraIRC.xml ""
  Rename $INSTDIR\HydraIRC.xml $INSTDIR\HydraIRC.old
  IfFileExists $INSTDIR\HydraIRC.new ""
  Delete $INSTDIR\HydraIRC.new

  ;Delete old plugins from ..\plugins\ folder if any
  IfFileExists $INSTDIR\plugins\plugin_*.dll "" +2
  Delete $INSTDIR\plugins\plugin_*.dll
  IfFileExists $INSTDIR\plugins\plugin.dll "" +2
  Delete $INSTDIR\plugins\plugin.dll

  File /r /x ".svn" "..\Distribution\Release\*.*" ;Extract files

  ;Create registry settings for future installations
  WriteRegStr HKLM "SOFTWARE\HydraProductions\${PRODUCT}" "InstallPath" $INSTDIR
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Preserve" "No"
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "StartMenu" "No"
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "QuickLaunch" "No"
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Desktop" "No"
  ;Create registry settings for Control Panel uninstall
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  ;Create registry settings for Support Information in Add/Remove Programs applet
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "InstallDate" "${__DATE__}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "HelpLink" "http://www.hydrairc.com/wiki/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Contact" "#hydrairc@EFnet"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Publisher" "Hydra Productions"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "URLInfoAbout" "http://www.hydrairc.com"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "URLUpdateInfo" "http://www.hydrairc.com"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Readme" "$INSTDIR\readme.txt"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "Comments" "The Professional IRC Client"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "InstallLocation" "$INSTDIR"
  ;Create uninstaller itself
  WriteUninstaller "uninstall.exe"
SectionEnd

Section "Use Previous Settings (Recommended)" SecPreserve

  ;This section, if used, will rename just installed .xml to .new
  ;and restore previous .old into .xml

  IfFileExists $INSTDIR\Profile_Default.old "" +4
  CopyFiles $INSTDIR\Profile_Default.xml $INSTDIR\Profile_Default.new
  CopyFiles $INSTDIR\Profile_Default.old $INSTDIR\Profile_Default.xml
  Delete $INSTDIR\Profile_Default.old

  IfFileExists $INSTDIR\HydraIRC.old "" +4
  CopyFiles $INSTDIR\HydraIRC.xml $INSTDIR\HydraIRC.new
  CopyFiles $INSTDIR\HydraIRC.old $INSTDIR\HydraIRC.xml
  Delete $INSTDIR\HydraIRC.old

  ;Remembers that this section was used during the installation
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Preserve" "Yes"
SectionEnd

Section "Start Menu Program Group" SecStartMenu
  CreateDirectory "$SMPROGRAMS\${PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\HydraIRC.lnk" "$INSTDIR\HydraIRC.exe" "" "$INSTDIR\HydraIRC.exe" 0
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\ReadMe.lnk" "$INSTDIR\ReadMe.txt" "" "$INSTDIR\ReadMe.txt" 0
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\Documentation.lnk" "$INSTDIR\Docs.htm" "" "$INSTDIR\Docs.htm" 0

  ;Remembers that this section was used during the installation
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "StartMenu" "Yes"
SectionEnd

Section "Quick Launch Shortcut" SecQuickLaunch
  CreateShortCut "$QUICKLAUNCH\HydraIRC.lnk" "$INSTDIR\HydraIRC.exe" "" "$INSTDIR\HydraIRC.exe" 0
  ;Remembers that this section was used during the installation
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "QuickLaunch" "Yes"
SectionEnd

Section "Desktop Shortcut" SecDesktop
  CreateShortCut "$DESKTOP\HydraIRC.lnk" "$INSTDIR\HydraIRC.exe" "" "$INSTDIR\HydraIRC.exe" 0
  ;Remembers that this section was used during the installation
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Desktop" "Yes"
SectionEnd
;<end> Sections (Install Options)
;-------------------------------

;<start> Description of sections
;-------------------------------
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} "The program itself. Required to use ${PRODUCT}."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecPreserve} "If installing into the same folder, this will make your previous settings active. Disabled otherwise."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} "Creates Start Menu program group and the shortcuts."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecQuickLaunch} "Creates a Quick Launch shortcut."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} "Creates a shortcut on the Desktop."
!insertmacro MUI_FUNCTION_DESCRIPTION_END
;<end> Description of sections
;-------------------------------


;<start> Functions
;-------------------------------
Function myPreComponentsFunction
;Needed to determine what sections (Components)
;were selected during the previous installation

  IfFileExists "$INSTDIR\*.xml" Registry Set
  ;Needed to determine if just selected $INSTDIR contains
  ;any xml files, and disable (or enable) "Preserve" option

  Set:
  SectionSetFlags ${SecPreserve} 17
  Goto End

  Registry:
  ;Checking if "Preserve" option was previously used
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Preserve"
  StrCmp $0 "No" +1 +2
  SectionSetFlags ${SecPreserve} 0
  StrCmp $0 "Yes" +1 +2
  SectionSetFlags ${SecPreserve} 1

  End:

  ;Checking if "StartMenu" option was previously used
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "StartMenu"
  StrCmp $0 "No" +1 +2
  SectionSetFlags ${SecStartMenu} 0
  StrCmp $0 "Yes" +1 +2
  SectionSetFlags ${SecStartMenu} 1

  ;Checking if "QuickLaunch" option was previously used
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "QuickLaunch"
  StrCmp $0 "No" +1 +2
  SectionSetFlags ${SecQuickLaunch} 0
  StrCmp $0 "Yes" +1 +2
  SectionSetFlags ${SecQuickLaunch} 1

  ;Checking if "Desktop" option was previously used
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "Desktop"
  StrCmp $0 "No" +1 +2
  SectionSetFlags ${SecDesktop} 0
  StrCmp $0 "Yes" +1 +2
  SectionSetFlags ${SecDesktop} 1
FunctionEnd

Function myPreFinishFunction
  ;Checking if Run box was previously checked
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRun"
  StrCmp $0 "No" +1 +2
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 4" "STATE" "0"
  ;Checking if Read box was previously checked
  ReadRegStr $0 HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRead"
  StrCmp $0 "No" +1 +2
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 5" "STATE" "0"
  ;To remove Cancel button from the FINISH page
  !insertmacro MUI_INSTALLOPTIONS_WRITE "ioSpecial.ini" "Settings" "CancelShow" "0"
FunctionEnd

Function myLeaveFinishFunction
  ;Remembering if Run box was checked during this instaltion
  !insertmacro MUI_INSTALLOPTIONS_READ $0 "ioSpecial.ini" "Field 4" "STATE"
  StrCmp $0 "0" +1 +2
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRun" "No"
  StrCmp $0 "1" +1 +2
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRun" "Yes"
  ;Remembering if Read box was checked during this instaltion
  !insertmacro MUI_INSTALLOPTIONS_READ $0 "ioSpecial.ini" "Field 5" "STATE"
  StrCmp $0 "0" +1 +2
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRead" "No"
  StrCmp $0 "1" +1 +2
  WriteRegStr HKCU "SOFTWARE\HydraProductions\${PRODUCT}" "FinishRead" "Yes"
FunctionEnd

;<end> Functions
;-------------------------------





;Uninstaller Section
;-------------------------------
Section Uninstall

  MessageBox MB_YESNOCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2 \
    "Remove ${PRODUCT} completely?$\n$\r\
    Yes - remove all, including your custom and/or received files$\n\
    No - keep files, but clean-up shortcuts and registry settings$\n\
    Cancel - abort uninstall process and make no changes at all" \
    IDYES unComplete IDNO unPartial

  Abort "Aborted by user - press Cancel to exit"

  unComplete:

  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION|MB_DEFBUTTON2 \
    "This will delete entire folder with all sub-folders:$\n\
     custom files, logs, downloads, etc. will be gone!$\n\
     Are you sure?" \
     IDOK unWipe

  Abort "Aborted by user - press Cancel to exit"

  unWipe:

  ;Deletes main folder, sub-folders, and files
  ;Delete $INSTDIR\*.*
  RMDir /r $INSTDIR
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"

  unPartial:

  ;Delete start menu stuff
  Delete "$SMPROGRAMS\${PRODUCT}\*.*"
  RMDir "$SMPROGRAMS\${PRODUCT}"

  ;Delete desktop and quick launch icons...
  Delete "$DESKTOP\HydraIRC.lnk"
  Delete "$QUICKLAUNCH\HydraIRC.lnk"

  ;Clean-up registry
  DeleteRegKey HKCU "SOFTWARE\HydraProductions\${PRODUCT}"
  DeleteRegKey HKLM "SOFTWARE\HydraProductions\${PRODUCT}"

SectionEnd
