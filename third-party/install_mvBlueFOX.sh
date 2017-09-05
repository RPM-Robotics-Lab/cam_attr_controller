#!/bin/bash
DEF_DIRECTORY=/opt/mvIMPACT_acquire
PRODUCT=mvBlueFOX
API=mvIMPACT_acquire
TARNAME=mvBlueFOX
USE_DEFAULTS=NO
APT_GET_EXTRA_PARAMS=

# Define the users real name if possible, to prevent accidental mvIA root ownership if script is invoked with sudo
if [ "$(which logname)" == "" ] ; then
    USER=$(whoami)
else
    if [ "$(logname 2>&1 | grep -c logname:)" == "1" ] ; then
        USER=$(whoami)
    else
        USER=$(logname)
    fi
fi

function createSoftlink {
    if [ ! -e "$1/$2" ]; then
        echo "Error: File "$1/$2" does not exist, softlink cannot be created! "
        exit 1
    fi
    if ! [ -L "$1/$3" ]; then
        ln -fs $2 "$1/$3" >/dev/null 2>&1
        if ! [ -L "$1/$3" ]; then
            sudo ln -fs $2 "$1/$3" >/dev/null 2>&1
            if ! [ -L "$1/$3" ]; then
                echo "Error: Could not create softlink $1/$3, even with sudo!"
                exit 1
            fi
        fi
    fi
} 

# Print out ASCII-Art Logo.
clear;
echo ""
echo ""
echo ""
echo ""
echo "                                 ===     ===      MMM~,    M                     "
echo "                                  ==+    ==       M   .M   M                     "
echo "                                  .==   .=+       M    M.  M   M    MM   ~MMM    "
echo "                                   ==+  ==.       MMMMM.   M   M    MM  M:   M   "
echo "              ..                   .== ,==        M   =M   M   M    MM +MMMMMMM  "
echo "    MMMM   DMMMMMM      MMMMMM      =====         M    MM  M   M    MM 7M        "
echo "    MMMM MMMMMMMMMMM :MMMMMMMMMM     ====         M    M+  M   MM  DMM  MM   ,   "
echo "    MMMMMMMMMMMMMMMMMMMMMMMMMMMMM                 IMM+''   M    .M:       =MI    "
echo "    MMMMMMM   .MMMMMMMM    MMMMMM                                                "
echo "    MMMMM.      MMMMMM      MMMMM                                                "
echo "    MMMMM       MMMMM       MMMMM                 MMMMMM    MMMMM    MM.   M     "
echo "    MMMMM       MMMMM       MMMMM                 M       MM    .MM  .M: .M      "
echo "    MMMMM       MMMMM       MMMMM                 M      .M       M~  .M~M       "
echo "    MMMMM       MMMMM       MMMMM                 MMMMMM MM       MD   +MM       "
echo "    MMMMM       MMMMM       MMMMM                 M       M       M    M MM      "
echo "    MMMMM       MMMMM       MMMMM                 M       MM     MM  :M. .M8     "
echo "    MMMMM       MMMMM       MMMMM                 M        .MMMMM    M     MD    "
echo "    MMMMM       MMMMM       MMMMM                                                "
echo ""
echo "=================================================================================="
sleep 1

# Analyze the command line arguments and react accordingly
PATH_EXPECTED=NO
SHOW_HELP=NO
while [[ $# -gt 0 ]] ; do
  if [ "$1" == "-h" ] || [ "$1" == "--help" ] ; then
    SHOW_HELP=YES
    break
  elif [[ ( "$1" == "-u" || "$1" == "--unattended" ) && "$PATH_EXPECTED" == "NO" ]] ; then
    USE_DEFAULTS=YES
  elif [[ ( "$1" == "-p" || "$1" == "--path" ) && "$PATH_EXPECTED" == "NO" ]] ; then
    if [ "$2" == "" ] ; then
      echo
      echo "WARNING: Path option used with no defined path, will use: $DEF_DIRECTORY directory"
      echo
      SHOW_HELP=YES
      break
    else
      PATH_EXPECTED=YES
    fi
  elif [ "$PATH_EXPECTED" == "YES" ] ; then
    DEF_DIRECTORY=$1
    PATH_EXPECTED=NO
  else
    echo 'Please check your syntax and try again!'
    SHOW_HELP=YES
  fi
  shift
done
if [ "$SHOW_HELP" == "YES" ] ; then
  echo
  echo 'Installation script for the '$PRODUCT' driver.'
  echo
  echo "Default installation path: "$DEF_DIRECTORY
  echo "Usage:                     ./install_mvBlueFOX.sh [OPTION] ... "
  echo "Example:                   ./install_mvBlueFOX.sh -p /myPath -u"
  echo
  echo "Arguments:"
  echo "-h --help                  Display this help."
  echo "-p --path                  Set the directory where the files shall be installed."
  echo "-u --unattended            Unattended installation with default settings."
  echo
  exit 1
fi
if [ "$USE_DEFAULTS" == "YES" ] ; then
  echo
  echo "Unattended installation requested, no user interaction will be required and the"
  echo "default settings will be used."
  echo
fi

# Get the targets platform and if it is called "i686" we know it is a x86 system, else it s x86_64
TARGET=$(uname -m)
if [ "$TARGET" == "i686" ]; then
   TARGET="x86"
fi

# Get the source directory (the directory where the files for the installation are) and cd to it
# (The script file must be in the same directory as the source TGZ) !!!
if which dirname >/dev/null; then
    SCRIPTSOURCEDIR="$PWD/$(dirname $0)"
fi
if [ "$SCRIPTSOURCEDIR" != "$PWD" ]; then
   if [ "$SCRIPTSOURCEDIR" == "" ] || [ "$SCRIPTSOURCEDIR" == "." ]; then
      SCRIPTSOURCEDIR="$PWD"
   fi
   cd "$SCRIPTSOURCEDIR"
fi

# Set variables for GenICam and mvIMPACT_acquire for later use
if grep -q '/etc/ld.so.conf.d/' /etc/ld.so.conf; then
   ACQUIRE_LDSOCONF_FILE=/etc/ld.so.conf.d/acquire.conf
else
   ACQUIRE_LDSOCONF_FILE=/etc/ld.so.conf
fi

# Make sure the environment variables are set at the next boot as well
if grep -q '/etc/profile.d/' /etc/profile; then
   ACQUIRE_EXPORT_FILE=/etc/profile.d/acquire.sh
else
   ACQUIRE_EXPORT_FILE=/etc/profile
fi

# Get driver name, version, file. In case of multiple *.tgz files in the folder select the newest version.
if [ "$( ls | grep -c 'mvBlueFOX.*\.tgz' )" != "0" ] ; then
  TARNAME=`ls mvBlueFOX*.tgz|tail -1 | sed -e s/\\.tgz//`
  TARFILE=`ls mvBlueFOX*.tgz|tail -1`
  VERSION=`ls mvBlueFOX*.tgz|tail -1 | sed -e s/\\mvBlueFOX// | sed -e s/\\-$TARGET// | sed -e s/\\_ABI2-// | sed -e s/\\.tgz//`
  ACT=$API-$TARGET-$VERSION
  ACT2=$ACT
fi

# Check if tar-file is correct for the system architecture
if [ "$TARGET" == "x86_64"  ]; then
  if [ "`echo $TARNAME | grep -c x86_ABI2`" != "0" ]; then
    echo "-----------------------------------------------------------------------------------"
    echo "  ABORTING: Attempt to install 32-bit drivers in a 64-bit machine!  " 
    echo "-----------------------------------------------------------------------------------"
    exit
  fi
fi
if [ "$TARGET" == "x86" ]; then
  if [ "`echo $TARNAME | grep -c x86_64_ABI2`" != "0" ]; then
    echo "-----------------------------------------------------------------------------------"
    echo "  ABORTING: Attempt to install 64-bit drivers in a 32-bit machine!  " 
    echo "-----------------------------------------------------------------------------------"
    exit
  fi
fi

# A quick check whether the Version has a correct format (due to other files being in the same directory..?)
if [ "$(echo $VERSION | grep -c '^[0-9]\{1,2\}\.[0-9]\{1,2\}\.[0-9]\{1,2\}')" == "0" ]; then
  echo "-----------------------------------------------------------------------------------"
  echo "  ABORTING: Script could not determine a valid mvIMPACT Acquire *.tgz file!  " 
  echo "-----------------------------------------------------------------------------------"
  echo "  This script could not extract a valid version number from the *.tgz file"
  echo "  This script determined $TARFILE as the file containing the installation data."
  echo "  It is recommended that only this script and the correct *.tgz file reside in this directory."
  echo "  Please remove all other files and try again."
  exit
fi

# A quick check whether the user has been determined
if [ "$USER" == "" ]; then
  echo "-----------------------------------------------------------------------------------"
  echo "  ABORTING: Script could not determine a valid user!  " 
  echo "-----------------------------------------------------------------------------------"
  echo "  This script could not determine the user of this shell"
  echo "  Please make sure this is a valid login shell!"
  exit
fi

YES_NO=
# Ask whether to use the defaults or proceed with an interactive installation
if [ "$USE_DEFAULTS" == "NO" ] ; then
  echo
  echo "Would you like this installation to run in unattended mode?"
  echo "No user interaction will be required, and the default settings will be used!"
  echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
  read YES_NO
else
  YES_NO=""
fi
if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
  USE_DEFAULTS=NO
else
  USE_DEFAULTS=YES
fi
 
# Here we will ask the user if we shall start the installation process
echo
echo "-----------------------------------------------------------------------------------"
echo "Configuration:"
echo "-----------------------------------------------------------------------------------"
echo
echo "Installation for user:            "$USER
echo "Installation directory:           "$DEF_DIRECTORY
echo "Source directory:                 "$(echo $SCRIPTSOURCEDIR | sed -e 's/\/\.//')
echo "Version:                          "$VERSION
echo "Platform:                         "$TARGET
echo "TAR-File:                         "$TARFILE
echo
echo "ldconfig:"
echo "mvIMPACT_acquire:                 "$ACQUIRE_LDSOCONF_FILE
echo
echo "Exports:"
echo "mvIMPACT_acquire:                 "$ACQUIRE_EXPORT_FILE
echo
echo "-----------------------------------------------------------------------------------"
echo
echo "Do you want to continue (default is 'yes')?"
echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
if [ "$USE_DEFAULTS" == "NO" ] ; then
  read YES_NO
else
  YES_NO=""
fi

# If the user is choosing no, we will abort the installation, else we will start the process.
if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
  echo "Quit!"
  exit
fi

# First of all ask whether to dispose of the old mvIMPACT Acquire installation
if [ "$MVIMPACT_ACQUIRE_DIR" != "" ]; then
  echo "Do you want to keep previous installation (default is 'yes')?"
  echo "If you select no, mvIMPACT Acquire will be removed for ALL installed Products!"
  echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
if [ "$USE_DEFAULTS" == "NO" ] ; then
  read YES_NO
else
  YES_NO=""
fi
  if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
    sudo rm -f /usr/bin/mvDeviceConfigure >/dev/null 2>&1
    sudo rm -f /usr/bin/mvIPConfigure >/dev/null 2>&1
    sudo rm -f /usr/bin/wxPropView >/dev/null 2>&1
    sudo rm -rf $MVIMPACT_ACQUIRE_DIR >/dev/null 2>&1
    if [ $? == 0 ]; then
      echo "Previous mvIMPACT Acquire Installation ($MVIMPACT_ACQUIRE_DIR) removed successfully!"
    else
      echo "Error removing previous mvIMPACT Acquire Installation ($MVIMPACT_ACQUIRE_DIR)!"
      echo "$?"
    fi
  else
    echo "Previous mvIMPACT Acquire Installation ($MVIMPACT_ACQUIRE_DIR) NOT removed!"
  fi
fi

# Create the *.conf files if the system is supporting ld.so.conf.d
if grep -q '/etc/ld.so.conf.d/' /etc/ld.so.conf; then
  sudo rm -f $ACQUIRE_LDSOCONF_FILE; sudo touch $ACQUIRE_LDSOCONF_FILE
fi

# Create the export files if the system is supporting profile.d
if grep -q '/etc/profile.d/' /etc/profile; then
  sudo rm -f $ACQUIRE_EXPORT_FILE; sudo touch $ACQUIRE_EXPORT_FILE
fi

# Check if the destination directory exist, else create it
if ! [ -d $DEF_DIRECTORY ]; then
  # the destination directory does not yet exist
  # first try to create it as a normal user
  mkdir -p $DEF_DIRECTORY >/dev/null 2>&1
  if ! [ -d $DEF_DIRECTORY ]; then
    # that didn't work
    # now try it as superuser
    sudo mkdir -p $DEF_DIRECTORY
  fi
  if ! [ -d $DEF_DIRECTORY  ]; then
    echo 'ERROR: Could not create target directory' $DEF_DIRECTORY '.'
    echo 'Problem:'$?
    echo 'Maybe you specified a partition that was mounted read only?'
    echo
    exit
  fi
else
  echo 'Installation directory already exists.'
fi

# in case the directory already existed BUT it belongs to other user
sudo chown -R $USER:$USER $DEF_DIRECTORY

# Check the actual tar-file
if ! [ -r $TARFILE ]; then
  echo 'ERROR: could not read' $TARFILE.
  echo
  exit
fi

# needed at compile time (used during development, but not shipped with the final program)
ACT=$API-$VERSION.tar

# Now unpack the tarfile into /tmp
cd /tmp
tar xfz "$SCRIPTSOURCEDIR/$TARFILE"

# Now check if we can unpack the tar file with the device independent stuff
# this is entirely optional
if [ -r /tmp/$ACT2 ]; then
   cd /tmp
   #tar xvf /tmp/$ACT
   cp -r $ACT2/* $DEF_DIRECTORY
else
  echo
  echo "ERROR: Could not read: /tmp/"$ACT2
  exit
fi

#Set the necessary exports and library paths
cd $DEF_DIRECTORY
if grep -q 'MVIMPACT_ACQUIRE_DIR=' $ACQUIRE_EXPORT_FILE; then
   echo 'MVIMPACT_ACQUIRE_DIR already defined in' $ACQUIRE_EXPORT_FILE.
else
   sudo sh -c "echo 'export MVIMPACT_ACQUIRE_DIR=$DEF_DIRECTORY' >> $ACQUIRE_EXPORT_FILE"
fi

if grep -q "$DEF_DIRECTORY/lib/$TARGET" $ACQUIRE_LDSOCONF_FILE; then
   echo "$DEF_DIRECTORY/lib/$TARGET already defined in" $ACQUIRE_LDSOCONF_FILE.
else
   sudo sh -c "echo '$DEF_DIRECTORY/lib/$TARGET' >> $ACQUIRE_LDSOCONF_FILE"
fi
if grep -q "$DEF_DIRECTORY/Toolkits/expat/bin/$TARGET/lib" $ACQUIRE_LDSOCONF_FILE; then
   echo "$DEF_DIRECTORY/Toolkits/expat/bin/$TARGET/lib already defined in" $ACQUIRE_LDSOCONF_FILE.
else
   sudo sh -c "echo '$DEF_DIRECTORY/Toolkits/expat/bin/$TARGET/lib' >> $ACQUIRE_LDSOCONF_FILE"
fi
if grep -q "$DEF_DIRECTORY/Toolkits/libudev/bin/$TARGET/lib" $ACQUIRE_LDSOCONF_FILE; then
   echo "$DEF_DIRECTORY/Toolkits/libudev/bin/$TARGET/lib already defined in" $ACQUIRE_LDSOCONF_FILE.
else
   sudo sh -c "echo '$DEF_DIRECTORY/Toolkits/libudev/bin/$TARGET/lib' >> $ACQUIRE_LDSOCONF_FILE"
fi

# This variable must be exported, or else wxPropView-related make problems can arise
export MVIMPACT_ACQUIRE_DIR=$DEF_DIRECTORY

# Set the libs to ldconfig
sudo /sbin/ldconfig

# Clen up /tmp
rm -r -f /tmp/$ACT2 /tmp/$API-$VERSION

# apt-get extra parameters
if [ "$USE_DEFAULTS" == "YES" ] ; then
  APT_GET_EXTRA_PARAMS=" -y --force-yes"
fi

# install needed libraries and compiler
COULD_NOT_INSTALL="Could not find apt-get or yast; please install >%s< manually."

# check if we have g++
if ! which g++ >/dev/null 2>&1; then
   if which apt-get >/dev/null 2>&1; then
      sudo apt-get $APT_GET_EXTRA_PARAMS -q install g++
   elif sudo which yast >/dev/null 2>&1; then
      YASTBIN=`sudo which yast`
      sudo $YASTBIN --install gcc-c++
   else
      printf "$COULD_NOT_INSTALL" "g++"
   fi
fi

if ! which make >/dev/null 2>&1; then
   if sudo which yast >/dev/null 2>&1; then
      YASTBIN=`sudo which yast`
      sudo $YASTBIN --install make
   else
      printf "$COULD_NOT_INSTALL" "make"
   fi
fi

# check if we have libexpat
if which apt-get >/dev/null 2>&1; then
  sudo apt-get $APT_GET_EXTRA_PARAMS -q install libexpat1-dev
elif sudo which yast >/dev/null 2>&1; then
  YASTBIN=`sudo which yast`
  sudo $YASTBIN --install expat libexpat-devel
else
  printf "$COULD_NOT_INSTALL" "libexpat"
fi

INPUT_REQUEST="Do you want to install >%s< (default is 'yes')?\nHit 'n' + <Enter> for 'no', or just <Enter> for 'yes'.\n"
YES_NO=

# do we want to install wxWidgets?
if [ "$(wx-config --release 2>&1 | grep -c "^3.")" != "1" ]; then
   echo
   printf "$INPUT_REQUEST" "wxWidgets"
   echo "This is highly recommended, as without wxWidgets, you cannot build wxPropView."
   echo
   if [ "$USE_DEFAULTS" == "NO" ] ; then
     read YES_NO
   else
     YES_NO=""
   fi
   if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
      echo 'Not installing wxWidgets'
   else
      if which apt-get >/dev/null 2>&1; then
         echo 'Installing wxWidgets'
         sudo apt-get $APT_GET_EXTRA_PARAMS -q install libwxgtk3.0-dev libwxbase3.0-0* libwxbase3.0-dev libwxgtk3.0-0* wx3.0-headers build-essential libgtk2.0-dev
      elif sudo which yast >/dev/null 2>&1; then
         echo 'Installing wxWidgets'
         YASTBIN=`sudo which yast`
         sudo $YASTBIN --install wxGTK-devel
      else
         printf "$COULD_NOT_INSTALL" "wxWidgets"
      fi
   fi
fi

# do we want to install FLTK?
if ! which fltk-config >/dev/null 2>&1; then
   echo
   printf "$INPUT_REQUEST" "FLTK"
   echo "This is only required if you want to build the 'LiveSnapFLTK' sample."
   echo
   if [ "$USE_DEFAULTS" == "NO" ] ; then
     read YES_NO
   else
     YES_NO=""
   fi
   if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
      echo 'Not installing FLTK'
   else
      if which apt-get >/dev/null 2>&1; then
         echo 'Installing FLTK'
         sudo apt-get $APT_GET_EXTRA_PARAMS -q install libgl1-mesa-dev
         sudo apt-get $APT_GET_EXTRA_PARAMS -q install libfltk1.1-dev
      elif sudo which yast >/dev/null 2>&1; then
         echo 'Installing FLTK'
         YASTBIN=`sudo which yast`
         sudo $YASTBIN --install Mesa-devel
         sudo $YASTBIN --install fltk-devel
      else
         printf "$COULD_NOT_INSTALL" "FLTK"
      fi
   fi
fi

echo
echo "Do you want the tools and samples to be built (default is 'yes')?"
echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
if [ "$USE_DEFAULTS" == "NO" ] ; then
  read YES_NO
else
  YES_NO=""
fi
if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
   echo 'The tools and samples were not built.'
   echo 'To build them yourself, type:'
   echo '  cd ~/mvimpact-acquire'
   echo "  make $TARGET"
   echo '  sudo /sbin/ldconfig'
else
   make $TARGET
   sudo /sbin/ldconfig

# Shall the MV tools be linked in /usr/bin?
   echo "Do you want to set a link to /usr/bin for wxPropView and mvDeviceConfigure (default is 'yes')?"
   echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
   if [ "$USE_DEFAULTS" == "NO" ] ; then
     read YES_NO
   else
     YES_NO=""
   fi
   if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
      echo "Will not set any new link to /usr/bin."
   else
      if [ -r /usr/bin ]; then
         # Set wxPropView
         if [ -r $DEF_DIRECTORY/apps/mvPropView/$TARGET/wxPropView ]; then
            sudo rm -f /usr/bin/wxPropView
            sudo ln -s $DEF_DIRECTORY/apps/mvPropView/$TARGET/wxPropView /usr/bin/wxPropView
         fi
         # Set mvDeviceConfigure
         if [ -r $DEF_DIRECTORY/apps/mvDeviceConfigure/$TARGET/mvDeviceConfigure ]; then
            sudo rm -f /usr/bin/mvDeviceConfigure
            sudo ln -s $DEF_DIRECTORY/apps/mvDeviceConfigure/$TARGET/mvDeviceConfigure /usr/bin/mvDeviceConfigure
         fi
      fi
   fi
fi

echo
echo "Do you want to copy 51-mvbf.rules to /etc/udev/rules.d for non-root user support (default is 'yes')?"
echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
if [ "$USE_DEFAULTS" == "NO" ] ; then
  read YES_NO
else
  YES_NO=""
fi
if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
   echo
   echo 'To grant non-root user support,'
   echo 'copy 51-mvbf.rules the file to /etc/udev/rules.d'
   echo
else
   sudo cp -f $DEF_DIRECTORY/Scripts/51-mvbf.rules /etc/udev/rules.d
fi

# check if plugdev group exists and the user is member of it
echo
if [ "$(grep -c plugdev /etc/group)" == "0" ]; then
  echo "Group 'plugdev' don't exists, this is necessary to run as non-root user, do you want to create it"
  echo "and add users to 'plugdev' (default is 'yes')?"
  echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
  if [ "$USE_DEFAULTS" == "NO" ] ; then
    read YES_NO
   else
     YES_NO=""
  fi
  if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
    echo
    echo "'plugdev' will be not created and you can't run the device as non-root user!"
    echo "If you want non-root users support, you will need to create 'plugdev'"
    echo "and add the users to this group."
  else
    sudo /usr/sbin/groupadd -g 46 plugdev
    sudo /usr/sbin/usermod -a $USER -G plugdev
    echo "Group 'plugdev' created and user '"$USER"' added to it."
  fi
else
  if [ "$(groups | grep plugdev -c)" == "0" ]; then
  sudo /usr/sbin/usermod -a $USER -G plugdev
  fi
fi

echo
echo "-----------------------------------------------------------------------------------"
echo "                            Installation successful!                               "
echo "-----------------------------------------------------------------------------------"
echo
echo "Do you want to reboot now (default is 'yes')?"
echo "Hit 'n' + <Enter> for 'no', or just <Enter> for 'yes'."
if [ "$USE_DEFAULTS" == "NO" ] ; then
  read YES_NO
else
  YES_NO="n"
fi
if [ "$YES_NO" == "n" ] || [ "$YES_NO" == "N" ]; then
   echo "You need to reboot manually to complete the installation."
else
   sudo shutdown -r now
fi
