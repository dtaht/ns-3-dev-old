#!/bin/bash

# Get the current repo name and version
# and format appropriately as a Javascript
# variable for inclusion in html files.

# Use cases:
# 1.  Hosted on nsnam.org, tagged release.
# 2.  Hosted on nsnam.org, ns-3-dev.
# 3.  User repo, at modified from a tagged release (or ns-3-dev).
# 4.  User repo, at a release tag.
# 5.  User repo, at a private tag.
# 6.  Private web host, at a tag (or ns-3-dev, or local mod).
#
# For case 1 and 2, we want the links to point to the nsnam.org
# publicly hosted pages.  For all other cases, we want to point
# to the built pages in the repo itself.
#
# The approach to identify cases 1 & 2 is to test:
# a.  We're on nsnam.org (actually, nsnam.ece.gatech.edu), and
# b.  We're in the tmp build directory, /tmp/daily-nsnam/
#     (This is the directory used by the update-* scripts
#     run by cron jobs.)
#
# If both a and b are true, we're building for nsnam.org.
#
# The repo version is either a tag name or a commit (short) id.
#
# If we're building for nsnam.org, and at a tag, we use just
# the tag as the repo name/version string, e.g. 'ns-3.14'.
# Otherwise, we're building for ns-3-dev, and we use, e.g,
# 'ns-3-dev @ fd0f27a10eff'.
#
# If we're *not* building for nsnam.org, we use the repo
# directory name as the repo name.  (This will typically be
# a name meaningful to the user doing the build, perhaps a
# shorthand for the feature they are working on.)  For
# example, this script was developed in a repo (mis)named
# 'doxygen'.  We always use the repo version, resulting
# in document version strings like 'doxygen @ ns-3.15' or
# 'doxygen @ fd0f27a10eff'
#

me=`basename $0`
function say
{
    echo "$me: $*"
}

function usage
{
    cat <<-EOF
	Usage:  $me                   normal versioning
	        $me [-n] [-d] [-t]    test options
	
	  -n  pretend we are on nsnam.org
	  -d  pretend we are in the automated build directory
	  -t  pretend we are at a repo tag
	    
EOF
    exit 1
}

# script arguments
say
nsnam=0
daily=0
tag=0

while getopts ndth option ; do
    case $option in
	(n)  nsnam=1 ;;

	(d)  daily=1 ;;

	(t)  tag=1   ;;

	(h | \? ) usage   ;;
    esac
done

# Hostname, fully qualified, e.g. nsnam.ece.gatech.edu
HOST=`hostname`
NSNAM="nsnam.ece.gatech.edu"

# Build directory
DAILY="^/tmp/daily-nsnam/"

if [ $nsnam -eq 1 ]; then
    HOST=$NSNAM
    say "-n forcing HOST = $HOST"
fi

if [ $daily -eq 1 ] ; then
    OLDPWD=$PWD
    PWD=/tmp/daily-nsnam/foo
    say "-d forcing PWD = $PWD"
fi

if [ $tag -eq 1 ]; then
    version="3.14"
    say "-t forcing tagged version = $version"
fi

if  ((nsnam + daily + tag > 0)) ; then
    say
fi

if [[ ( $HOST == $NSNAM ) && ( $PWD =~ $DAILY ) ]] ; then
    PUBLIC=1
    say "building public docs for nsnam.org"
else
    PUBLIC=0
    say "building private docs"
fi

if [ $daily -eq 1 ]; then
    PWD=$OLDPWD
fi

# Destination javascript file
outf="doc/ns3_html_theme/static/ns3_version.js"

# Distance from last tag
# Zero distance means we're at the tag
distance=`hg log -r tip --template '{latesttagdistance}'`

if [ $distance -eq 0 ]; then
    version=`hg log -r tip --template '{latesttag}'`
    say "at tag $version"

elif [ $tag -eq 1 ]; then
    distance=0
    version="3.14"

else
    version=`hg log -r tip --template '{node|short}'`
    # Check for uncommitted changes
    hg summary | grep -q 'commit: (clean)'
    if [ ! $? ] ; then
	say "beyond latest tag, last commit: $version, dirty"
	version="$version(+)"
    else
	say "beyond latest tag, last commit: $version, clean"
    fi
fi

if [ $PUBLIC -eq 1 ]; then
    echo "var ns3_host = \"http://www.nsnam.org/\";"         >  $outf
    
    if [ $distance -eq 0 ]; then
	echo "var ns3_version = \"Release $version\";"       >> $outf
	echo "var ns3_release = \"docs/release/$version/\";" >> $outf
    else
	echo "var ns3_version = \"ns-3-dev @ $version\";"    >> $outf
	echo "var ns3_release = \"docs/\";" >> $outf
    fi
    echo "var ns3_local = \"\";"                             >> $outf
    echo "var ns3_doxy  = \"doxygen/\";"                     >> $outf
    
else
    repo=`basename $PWD`
    echo "var ns3_host = \"file://$PWD/\";"                  >  $outf
    echo "var ns3_version = \"$repo @ $version\";"           >> $outf
    echo "var ns3_release = \"doc/\";"                       >> $outf
    echo "var ns3_local = \"build/\";"                       >> $outf
    echo "var ns3_doxy  = \"html/\";"                        >> $outf
fi

# Copy to html directories
# This seems not always done automatically
# by Sphinx when rebuilding
cd doc 2>&1 >/dev/null
for d in {manual,models,tutorial{,-pt-br}}/build/{single,}html/_static html \
    html ; do
    # expect the copy to fail if the destination dir
    # hasn't been created by a prior doc build
    cp ns3_html_theme/static/ns3_version.js $d 2>/dev/null
done
cd - 2>&1 >/dev/null

# Show what was done
say
say "outf = $outf:"
cat -n $outf


