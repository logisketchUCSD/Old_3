$Id: ReadMe.txt,v 1.3 2006/11/03 07:52:27 weesan Exp $

User study
----------
01 - weesan
02 - tom4
03 - ya_lee2
04 - rumi2
05 - crystal
06 - teddy
07 - eva
08 - Anthony
09 - San

Filelist
--------
ReadMe.txt						- This file.
algebra.cpp						- Some convenient algebra functions.
algebra.h
assigned_match.cpp		- Matching by hand-assignment.
assigned_match.h
bgi_font.cpp					- A class to decode Borland's fonts.
bgi_font.h
confusion_matrix.cpp	- Confusion matrix class implementation.
confusion_matrix.h
Coord.cpp							- A class for x, y coord.
Coord.h
curve.h								- A class for arc, but Arc is taken, thus, Curve.
data/									- Definitions and symbols
debug.cpp							- Debug facilities.
debug.h
def_viewer.cpp				- Definition viewer.
def_viewer.h
definition.cpp				- Definition class.
definition.h
directory.cpp					- Some convenient function/class to manipulate directories.
directory.h
dist_prob.cpp					- A class to implement distribution probability.
dist_prob.h
distance.cpp					- A class to compute "distance" between 2 symbols.
distance.h
distance_match.cpp		- Matching using "distance".
distance_match.h
docs/									- Some useful documentations.
extern.h
fonts/								- Borland's fonts.
GetOpt.cpp						- C++ version of getopt().
getopt.h
globals.h							- Some global varibles/constants.
glue.cpp							- The "glue" layer.
glue.h
graph_based.aps				- VC++/.Net stuff
graph_based.clw
graph_based.cpp
graph_based.dsp
graph_based.dsw
graph_based.h
graph_based.ncb
graph_based.opt
graph_based.plg
graph_based.rc
graph_based.sln
graph_based.suo
graph_based.vcproj
graph_based_doc.cpp
graph_based_doc.h
graph_based_main.cpp	
graph_based_main.h
graph_based_view.cpp
graph_based_view.h
greedy_match.cpp			- Matching by using greedy search.
greedy_match.h
infix2postfix.cpp			- A fun, small code snipet to convert infix to postfix notation.
infix2postfix.h
line.h								- A line class.
match.cpp							- Base class for various matching methods.
match.h
matrix.cpp						- A matrix class.
matrix.h
null_match.cpp				- Matching by original drawing order.
null_match.h
object.h							- Base object class.
options.cpp						- Option class to keep track of command-line options.
options.h
population.cpp				- A way to compute mean and sigma.
population.h
prob_match.cpp				- Error-driven/probability matching.
prob_match.h
random_match.cpp			- Random matching.
random_match.h
recog_viewer.cpp			- Recognition result viewer.
recog_viewer.h
recognizer.cpp				- The recognizer.
recognizer.h
relationship.cpp			- A class to implement the graph relationship.
relationship.h
res/									- Windows resource files.
resource.h
result.cpp						- A class to keep track of accurary and elapsed time for recognition.
result.h
run.bat								- Scripts to invoke the recognizer.
run_random.bat
scandir.cpp						- Scan directory for filenames.
scandir.h
segment.cpp						- Segments of the symbols.
segment.h
sort_match.cpp				- Sort matching.
sort_match.h
sound.cpp							- A class to play .wav files.
sound.h
sounds/								- The sound files.
split.l
stats.h								- Another way to compute mean and sigma.
StdAfx.cpp
StdAfx.h
symbol.cpp						- The symbol class.
symbol.h
symbol_viewer.cpp			- The symbol viewer.
symbol_viewer.h
thread.cpp						- A thread class.
thread.h
timer.h								- A timer class.
training/							- Gif files for the symbols during training.
training.cpp					- A class to read gif files and some training related functions.
training.h
user_study.14					- User study result.
user_study.orig
user_study_best_3
user_study_best_3.14
utils.h								- Some utilities.
vector.h							- Vector implementation.
wintab32.lib					- The library to communicate with watcom driver.
ysmain.cpp						- The console hack.


Some notes from weesan
----------------------
o Create a new SDI project
o Copy WINTAB32.LIB from Prof. Stahovich's code
o Add the following extra libaries into Project Settings/Link/Object or 
	Library modules: opengl32.lib glu32.lib glaux.lib wintab32.lib 
o Add an include path to whatever the code from Prof. Stahovich into 
	the Project Settings/C or C++/Preprocessor/Addition include directories.
	eg. ..\3dpen-v3-tablet-ver3
o Add the following files from Prof. Stahovich's code to the project's new
  directory call "External Source Files":
	COpenGLView.cpp
	gl_view.cpp
	ink.cpp
	OPENGL VIEW CLASSDOC.CPP
	PlotDialog.cpp
	ProcessDialog.cpp
	PsRender.cpp
	pythag.cpp
	svbksb.cpp
	SVDC_SolveAXD.cpp
	svdcmp.cpp
o Commented out the App from Prof. Stahovich's code
o Inherit View and Doc
o Have inherited View class's OnDraw() calls its parent's OnDraw()


========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : graph_based
========================================================================


AppWizard has created this graph_based application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your graph_based application.

graph_based.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

graph_based.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CGraphBasedApp application class.

graph_based.cpp
    This is the main application source file that contains the application
    class CGraphBasedApp.

graph_based.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

graph_based.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\graph_based.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file graph_based.rc.

res\graph_based.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.



/////////////////////////////////////////////////////////////////////////////

For the main frame window:

graph_based_main.h, graph_based_main.cpp
    These files contain the frame class CGraphBasedMain, which is derived from
    CFrameWnd and controls all SDI frame features.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

graph_based_doc.h, graph_based_doc.cpp - the document
    These files contain your CGraphBasedDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CGraphBasedDoc::Serialize).

graph_based_view.h, graph_based_view.cpp - the view of the document
    These files contain your CGraphBasedView class.
    CGraphBasedView objects are used to view CGraphBasedDoc objects.



/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named graph_based.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////
