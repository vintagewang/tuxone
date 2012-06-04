#!/bin/sh

CWD_DIR=`pwd`
TMP_DIR=`date +%Y%m%d_%H%M%S`

SRC_DIR=${CWD_DIR}/../code
TAG_DIR=${CWD_DIR}/${LWPR_ENV_ARCH}_${TMP_DIR}
XML_DIR=${XERCES_HOME}/${LWPR_ENV_ARCH}

if [ ${LWPR_ENV_ARCH} = "32" ]
then
    TAR_NAME=tuxone-${LWPR_ENV_VERSION}-linux-x86
else
    TAR_NAME=tuxone-${LWPR_ENV_VERSION}-linux-x64
fi

mkdir ${TAG_DIR}
mkdir ${TAG_DIR}/tmp
mkdir ${TAG_DIR}/tmp/bin
mkdir ${TAG_DIR}/tmp/include
mkdir ${TAG_DIR}/tmp/lib
mkdir ${TAG_DIR}/tmp/udataobj
mkdir ${TAG_DIR}/tmp/docs

cp ${SRC_DIR}/bin/buildclient       ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/buildserver       ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/tmadmin           ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/tmshutdown        ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/txgate            ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/viewc32           ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/viewdis32         ${TAG_DIR}/tmp/bin/
cp ${SRC_DIR}/bin/mkfldhdr32        ${TAG_DIR}/tmp/bin/

cp ${SRC_DIR}/lib/liblwpr.so        ${TAG_DIR}/tmp/lib/
cp ${SRC_DIR}/lib/libtux.so         ${TAG_DIR}/tmp/lib/
cp ${SRC_DIR}/lib/libtmb.so         ${TAG_DIR}/tmp/lib/

cp ${SRC_DIR}/include/atmi.h        ${TAG_DIR}/tmp/include/
cp ${SRC_DIR}/include/userlog.h     ${TAG_DIR}/tmp/include/
cp ${SRC_DIR}/include/fml32.h       ${TAG_DIR}/tmp/include/
cp ${SRC_DIR}/include/fml1632.h     ${TAG_DIR}/tmp/include/
cp -R ${SRC_DIR}/samples            ${TAG_DIR}/tmp/
rm -f ${TAG_DIR}/tmp/samples/atmi/echo/cmpl.sh
rm -f ${TAG_DIR}/tmp/samples/atmi/fml32/cmpl.sh
rm -rf ${TAG_DIR}/tmp/samples/atmi/simpapp
rm -rf ${TAG_DIR}/tmp/samples/atmi/xmlstockapp

## For 3rd_party_licenses
cp ${CWD_DIR}/3rd_party_licenses.txt	${TAG_DIR}/tmp/udataobj/

## For xml
cp ${XML_DIR}/lib/libxerces-c.so    ${TAG_DIR}/tmp/lib/libtxml.so
cp -R ${XML_DIR}/include/xercesc    ${TAG_DIR}/tmp/include/

## For docs
cp ${CWD_DIR}/../docs/tuxone_developing_guide.pdf	${TAG_DIR}/tmp/docs/

cp tux.env                          ${TAG_DIR}/tmp/
cp ubbconfig                        ${TAG_DIR}/tmp/
cp install.sh                       ${TAG_DIR}/

cd ${TAG_DIR}/tmp
tar cvf ${TAG_DIR}/tuxone_install.tar *

cd ${TAG_DIR}
tar cvf ${TAR_NAME}.tar             tuxone_install.tar install.sh
gzip ${TAR_NAME}.tar
