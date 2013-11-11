# Microsoft Developer Studio Project File - Name="graph_based" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=graph_based - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "graph_based.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "graph_based.mak" CFG="graph_based - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "graph_based - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "graph_based - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "graph_based - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 -lm

!ELSEIF  "$(CFG)" == "graph_based - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "." /I "..\3dpen-v3-tablet-ver3" /I "..\fltk-1.1.6" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib wintab32.lib fltkd.lib fltkimagesd.lib wsock32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\fltk-1.1.6\lib"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "graph_based - Win32 Release"
# Name "graph_based - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\algebra.cpp
# End Source File
# Begin Source File

SOURCE=.\assigned_match.cpp
# End Source File
# Begin Source File

SOURCE=.\bgi_font.cpp
# End Source File
# Begin Source File

SOURCE=.\confusion_matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Coord.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\def_viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\definition.cpp
# End Source File
# Begin Source File

SOURCE=.\directory.cpp
# End Source File
# Begin Source File

SOURCE=.\dist_prob.cpp
# End Source File
# Begin Source File

SOURCE=.\distance.cpp
# End Source File
# Begin Source File

SOURCE=.\distance_match.cpp
# End Source File
# Begin Source File

SOURCE=.\GetOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\glue.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_based.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_based.rc
# End Source File
# Begin Source File

SOURCE=.\graph_based_doc.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_based_main.cpp
# End Source File
# Begin Source File

SOURCE=.\graph_based_view.cpp
# End Source File
# Begin Source File

SOURCE=.\greedy_match.cpp
# End Source File
# Begin Source File

SOURCE=.\infix2postfix.cpp
# End Source File
# Begin Source File

SOURCE=.\match.cpp
# End Source File
# Begin Source File

SOURCE=.\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\null_match.cpp
# End Source File
# Begin Source File

SOURCE=.\options.cpp
# End Source File
# Begin Source File

SOURCE=.\population.cpp
# End Source File
# Begin Source File

SOURCE=.\prob_match.cpp
# End Source File
# Begin Source File

SOURCE=.\random_match.cpp
# End Source File
# Begin Source File

SOURCE=.\recog_viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\recognizer.cpp
# End Source File
# Begin Source File

SOURCE=.\relationship.cpp
# End Source File
# Begin Source File

SOURCE=.\result.cpp
# End Source File
# Begin Source File

SOURCE=.\scandir.cpp
# End Source File
# Begin Source File

SOURCE=.\segment.cpp
# End Source File
# Begin Source File

SOURCE=.\sort_match.cpp
# End Source File
# Begin Source File

SOURCE=.\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\symbol_viewer.cpp
# End Source File
# Begin Source File

SOURCE=.\thread.cpp
# End Source File
# Begin Source File

SOURCE=.\training.cpp
# End Source File
# Begin Source File

SOURCE=.\ysmain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\algebra.h
# End Source File
# Begin Source File

SOURCE=.\assigned_match.h
# End Source File
# Begin Source File

SOURCE=.\bgi_font.h
# End Source File
# Begin Source File

SOURCE=.\confusion_matrix.h
# End Source File
# Begin Source File

SOURCE=.\Coord.h
# End Source File
# Begin Source File

SOURCE=.\curve.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\def_viewer.h
# End Source File
# Begin Source File

SOURCE=.\definition.h
# End Source File
# Begin Source File

SOURCE=.\directory.h
# End Source File
# Begin Source File

SOURCE=.\dist_prob.h
# End Source File
# Begin Source File

SOURCE=.\distance.h
# End Source File
# Begin Source File

SOURCE=.\distance_match.h
# End Source File
# Begin Source File

SOURCE=.\extern.h
# End Source File
# Begin Source File

SOURCE=.\getopt.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\glue.h
# End Source File
# Begin Source File

SOURCE=.\graph_based.h
# End Source File
# Begin Source File

SOURCE=.\graph_based_doc.h
# End Source File
# Begin Source File

SOURCE=.\graph_based_main.h
# End Source File
# Begin Source File

SOURCE=.\graph_based_view.h
# End Source File
# Begin Source File

SOURCE=.\greedy_match.h
# End Source File
# Begin Source File

SOURCE=.\infix2postfix.h
# End Source File
# Begin Source File

SOURCE=.\line.h
# End Source File
# Begin Source File

SOURCE=.\match.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\null_match.h
# End Source File
# Begin Source File

SOURCE=.\object.h
# End Source File
# Begin Source File

SOURCE=.\options.h
# End Source File
# Begin Source File

SOURCE=.\population.h
# End Source File
# Begin Source File

SOURCE=.\prob_match.h
# End Source File
# Begin Source File

SOURCE=.\random_match.h
# End Source File
# Begin Source File

SOURCE=.\recog_viewer.h
# End Source File
# Begin Source File

SOURCE=.\recognizer.h
# End Source File
# Begin Source File

SOURCE=.\relationship.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\result.h
# End Source File
# Begin Source File

SOURCE=.\scandir.h
# End Source File
# Begin Source File

SOURCE=.\segment.h
# End Source File
# Begin Source File

SOURCE=.\sort_match.h
# End Source File
# Begin Source File

SOURCE=.\sound.h
# End Source File
# Begin Source File

SOURCE=.\stats.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\symbol.h
# End Source File
# Begin Source File

SOURCE=.\symbol_viewer.h
# End Source File
# Begin Source File

SOURCE=.\thread.h
# End Source File
# Begin Source File

SOURCE=.\timer.h
# End Source File
# Begin Source File

SOURCE=.\training.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\graph_based.ico
# End Source File
# Begin Source File

SOURCE=.\res\graph_based.rc2
# End Source File
# Begin Source File

SOURCE=.\res\graph_based_doc.ico
# End Source File
# End Group
# Begin Group "External Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\COpenGLView.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\gl_view.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\ink.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\OPENGL VIEW CLASSDOC.CPP"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\PlotDialog.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\ProcessDialog.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\PsRender.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\pythag.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\svbksb.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\SVDC_SolveAXD.cpp"
# End Source File
# Begin Source File

SOURCE="..\3dpen-v3-tablet-ver3\svdcmp.cpp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
