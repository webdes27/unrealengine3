#!################################################################################
#! File:    x11.t
#! Purpose: tmake template file from which src/x11/files.lst containing the
#!          list of files for wxMotif library is generated by tmake
#! Author:  Vadim Zeitlin
#! Created: 28.01.00
#! Version: $Id: x11.t,v 1.5.2.1 2002/11/05 10:02:46 RL Exp $
#!################################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric, %wxHtml, %wxUNIX, %wxMOTIF, %wxX11 and
    #! %wxOS2PM hashes.
    IncludeTemplate("filelist.t");

    #! find all our sources
    foreach $file (sort keys %wxX11) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"X11_SOURCES"} .= "x11/" . $file . " ";
        $project{"GUIOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\bNotX11\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"X11_SOURCES"} .= "generic/" . $file . " ";
        $project{"GENERICOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\bNotX11\b/;

        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"X11_SOURCES"} .= "common/" . $file . " ";
        $project{"COMMONOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxUNIX) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"X11_SOURCES"} .= "unix/" . $file . " ";
        $project{"UNIXOBJS"} .= $fileobj . " ";
    }

    foreach $file (sort keys %wxHTML) {
        ($fileobj = $file) =~ s/cp?p?$/\o/;

        $project{"X11_SOURCES"} .= "html/" . $file . " ";
        $project{"HTMLOBJS"} .= $fileobj . " ";
    }
    #! find all our headers
    foreach $file (sort keys %wxWXINCLUDE) {
        next if $wxWXINCLUDE{$file} =~ /\bNotX11\b/;

        $project{"X11_HEADERS"} .= $file . " "
    }

    foreach $file (sort keys %wxX11INCLUDE) {
        $project{"X11_HEADERS"} .= "x11/" . $file . " "
    }

    foreach $file (sort keys %wxUNIVINCLUDE) {
        $project{"X11_HEADERS"} .= "univ/" . $file . " ";
    }

    foreach $file (sort keys %wxGENERICINCLUDE) {
        $project{"X11_HEADERS"} .= "generic/" . $file . " "
    }

    foreach $file (sort keys %wxUNIXINCLUDE) {
        $project{"X11_HEADERS"} .= "unix/" . $file . " "
    }

    foreach $file (sort keys %wxHTMLINCLUDE) {
        $project{"X11_HEADERS"} .= "html/" . $file . " "
    }

    foreach $file (sort keys %wxPROTOCOLINCLUDE) {
        $project{"X11_HEADERS"} .= "protocol/" . $file . " "
    }
#$}
# This file was automatically generated by tmake 
# DO NOT CHANGE THIS FILE, YOUR CHANGES WILL BE LOST! CHANGE X11.T!
ALL_SOURCES = \
		#$ ExpandList("X11_SOURCES");

ALL_HEADERS = \
		#$ ExpandList("X11_HEADERS");

GUI_LOWLEVEL_OBJS = \
		#$ ExpandList("GUIOBJS");

COMMONOBJS = \
		#$ ExpandList("COMMONOBJS");

GENERICOBJS = \
		#$ ExpandList("GENERICOBJS");

UNIXOBJS = \
		#$ ExpandList("UNIXOBJS");

HTMLOBJS = \
		#$ ExpandList("HTMLOBJS");

