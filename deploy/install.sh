#!/bin/sh
##
## Tuxone Install Script
##

PATH=$PATH:/bin:/usr/bin:/sbin:/usr/sbin:/usr/local/bin

## Define Keywords
TUXDIR_KW="<Tuxone Home Dir>"

## Define Functions
setValue(){
	#set value
	sed "s^$1^$2^g" $3 >${3}.bk

	#move file
	mv ${3}.bk $3
}


## STEP 1 - Read Install Directory
echo
echo -n "Please input install directory:"
read INSTALL_DIR

while [ -z "$INSTALL_DIR" ]
do
	echo
	echo -n "Empty value is not accepted.Please input install directory:"
	read INSTALL_DIR
done

if [ ! -d "$INSTALL_DIR" ]
then
	echo
	echo "The directory $INSTALL_DIR does not exist!"
	echo
	while true
	do
		echo -n "Do you want to create it?(Y/N default:Y):"
		read CHOICE

		if [ ! -z "$CHOICE" ]
		then
			if [ $CHOICE = n -o $CHOICE = N ]
			then
				echo "The installation has been canceled..."
				exit 1
			elif [ $CHOICE = y -o $CHOICE = Y ]
			then
				break
			else
				echo "\nInvalid choice, please select again.\n"
				continue
			fi
		fi
		break
	done

	#create install directory
	mkdir -p $INSTALL_DIR

	if [ $? -ne 0 ] ; then
		echo "Failed to create directory $INSTALL_DIR,so quit..."
		exit 1
	fi
fi

case $INSTALL_DIR in
	/*);;
	*)
		INSTALL_DIR=`pwd`/$INSTALL_DIR
esac

## STEP 2 - Copy File To The Install Directory
echo
echo "Begin to install the software..."

BACK_DIR=`pwd`
cd "$INSTALL_DIR"
tar xvf ${BACK_DIR}/tuxone_install.tar >/dev/null 2>&1
if [ $? -ne 0 ] ; then
	echo
	echo "Unpacking file failed!"
	echo "Please make sure you have permission to write the directory $INSTALL_DIR."
	echo "and you have enough space in install disk."
	echo
	cd "$BACK_DIR"
	exit 1
fi
cd "$BACK_DIR"

## STEP 3 - Begin To Config The Product

# set tuxone home
setValue "${TUXDIR_KW}" "${INSTALL_DIR}" "${INSTALL_DIR}/tux.env"

# set file permission
chmod 755 ${INSTALL_DIR}/bin/*
chmod 755 ${INSTALL_DIR}/tux.env

# print last message
echo
echo "Congratulations. TUXONE has been successfully installed to:"
echo
echo "	${INSTALL_DIR}"
exit 0
