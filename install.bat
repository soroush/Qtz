

xcopy /s /y /v qtz\core\*.h* c:\sysroot\include\qtz\core
xcopy /s /y /v qtz\data\*.h* c:\sysroot\include\qtz\data
xcopy /s /y /v qtz\security\*.h* c:\sysroot\include\qtz\security
xcopy /s /y /v qtz\widgets\*.h* c:\sysroot\include\qtz\widgets
xcopy /s /y /v qtz\widgets\data\*.h* c:\sysroot\include\qtz\widgets\data
xcopy /s /y /v qtz\widgets\editors\*.h* c:\sysroot\include\qtz\widgets\editors
xcopy /s /y /v qtz\widgets\misc\*.h* c:\sysroot\include\qtz\widgets\misc
xcopy /s /y /v qtz\widgets\security\*.h* c:\sysroot\include\qtz\widgets\security

xcopy /s /y /v ..\qtz-project-release\qtz\core\release\QtzCore0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-release\qtz\data\release\QtzData0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-release\qtz\security\release\QtzSecurity0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-release\qtz\widgets\release\QtzWidgets0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-release\qtz\core\release\QtzCore0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-release\qtz\data\release\QtzData0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-release\qtz\security\release\QtzSecurity0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-release\qtz\widgets\release\QtzWidgets0.lib c:\sysroot\lib

xcopy /s /y /v ..\qtz-project-debug\qtz\core\debug\QtzCored0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-debug\qtz\data\debug\QtzDatad0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-debug\qtz\security\debug\QtzSecurityd0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-debug\qtz\widgets\debug\QtzWidgetsd0.dll c:\sysroot\bin
xcopy /s /y /v ..\qtz-project-debug\qtz\core\debug\QtzCored0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-debug\qtz\data\debug\QtzDatad0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-debug\qtz\security\debug\QtzSecurityd0.lib c:\sysroot\lib
xcopy /s /y /v ..\qtz-project-debug\qtz\widgets\debug\QtzWidgetsd0.lib c:\sysroot\lib
