/*	(c) 2003 BEA Systems, Inc. All Rights Reserved. */
#ifdef __cplusplus
#include <xercesc/util/PlatformUtils.hpp>
extern "C" {
#endif
extern void parseXMLBuffer(void *handle, char** xmlbuf);
extern struct wrapper* getHandle();
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
class wrapper {
   public:
     void parseXMLBuffer(char** xmlfile);
};
#endif
