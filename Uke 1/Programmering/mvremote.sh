#!/bin/bash

#   Course:     IN2140
#   File:       mvremote.sh
#   Synopsis:   This is a bash script that helps you move your assignment code 
#               to a remote IFI machine from your local machine.
#
#   ---------------------------------------------------------------------------
#
#   Usage:
#   ./mvremote.sh -u <UIO_USERNAME> -p <PATH_TO_PROJECT_DIR> -d <DIR_ON_IFI_MACHINES>
#
#   <UIO_USERNAME>          Your UiO username in lower case letters.
#   <PATH_TO_PROJECT_DIR>   Path to where your prjoecjt files are located.
#   <DIR_ON_IFI_MACHINES>   The directory on the ifi machines where you want
#                           your project files to be placed.
#   
#   NOTE:
#   The directory specified on the IFI machines must be av valid path. 
#   The script always assumes the starting point of the path to the directory 
#   as ~/Documents/. "~" is your home directory which can also be refed to 
#   as $HOME.
#
#   ---------------------------------------------------------------------------
#
#   What the script does
#
#   1) Set up default variables.
#   2) Parse arguments.
#   3) Set variables.
#   4) Moves your project files to the specified directory on the remote host.
#   5) Starts a SSH session on the remote machine in the specfied directory.
#
#   ---------------------------------------------------------------------------
#
#   Feel free to modify this script as much as you want, at your own expence.
#   Use your time wisely! :)

usage() {
    echo -e "\n$0 -u <UIO_USERNAME> -p <PATH_TO_PRJOCET_DIR> -d <PATH_TO_REMOTE_DIR>"
    echo -e "\n<UIO_USERNAME>\t\tYour UiO username in lower case letters."
    echo -e "<PATH_TO_PROJECT_DIR>\tPath to where your project files are located."
    echo -e "<PATH_TO_REMOTE_DIR>\tThe directory on the IFI machines where you want your project files to be placed."
    echo -e "\nNOTE:
        The directory specified on the IFI machines must be av valid path.
        The script always assumes the starting point of the path to the directory 
        as ~/Documents/. "~" is your home directory which can also be refed to
        as $HOME."
}

correct_path() {
    local str=$1
    local last=$((${#str}-1))

    if [ "${str:0:1}" == "/" ]; then 
        str="${str:1:last}"
        last=$((${#str}-1))
    fi

    if [ "${str:last:1}" != "/" ]; then
        str="$str/"
    fi

    echo "$str"
}

#
#   1) Set default variables and values
#

UIO_USERNAME=
PROJECT_DIR=
REMOTE_DIR=
DEFAULT_REMOTE_DIR="~/Documents"

#
#   2) Parse args & 3) Set variables
#

while getopts ":u:p:d:" o; do
    case "${o}" in
        u)
            UIO_USERNAME="${OPTARG}"
            ;;
        p)
            PROJECT_DIR="$(correct_path ${OPTARG})"
            ;;
        d)
            REMOTE_DIR="$DEFAULT_REMOTE_DIR/$(correct_path ${OPTARG})"
            ;;
        h | *)
            usage
            exit 0
            ;;
    esac
done

shift $((OPTIND-1))

if [ -z "$UIO_USERNAME" ] || [ -z "$PROJECT_DIR" ] || [ -z "$REMOTE_DIR" ]; then
    usage
    exit 0
fi

#
#   4) Move project files to remote directory
#

echo -e "\nCopying $PROJECT_DIR to $UIO_USERNAME@login.ifi.uio.no:$REMOTE_DIR"
scp -r $PROJECT_DIR $UIO_USERNAME@login.ifi.uio.no:$REMOTE_DIR

#
#   5) Start SSH session in remote directory
#

echo -e "\nStarting remote session. $UIO_USERNAME@login.ifi.uio.no:$REMOTE_DIR$PROJECT_DIR"
ssh -t $UIO_USERNAME@login.ifi.uio.no "cd $REMOTE_DIR$PROJECT_DIR ; bash"