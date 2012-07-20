/*
 * Copyright 2011, The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "WimaxJNI"

#include "jni.h"
#include <utils/misc.h>
#include <utils/Log.h>

//#include <netutils/ifc.h>
#include "cutils/properties.h"

#include <stdlib.h>
#include <string.h>

#include "wimax.h"

#ifdef HAVE_LIBC_SYSTEM_PROPERTIES
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#endif

#define WIMAX_PKG_NAME "com/htc/net/wimax/WimaxNative"
typedef unsigned char byte;

extern "C" {

static struct fieldIds {
    jclass dhcpInfoClass;
    jmethodID constructorId;
    jfieldID ipaddress;
    jfieldID gateway;
    jfieldID netmask;
    jfieldID dns1;
    jfieldID dns2;
    jfieldID serverAddress;
    jfieldID leaseDuration;
} dhcpInfoFieldIds;

// load driver / two versions for compat
static jboolean com_htc_net_wimax_startDriver(JNIEnv* env, jobject clazz)
{
    int rval = ::loadWimaxDriver();

    return (jboolean)(rval == 0);
}


static jboolean com_htc_net_wimax_loadWimaxDriver(JNIEnv* env, jobject clazz)
{
    int rval = ::loadWimaxDriver();

    return (jboolean)(rval == 0);
}

// unload driver / two versions for compat 
static jboolean com_htc_net_wimax_stopDriver(JNIEnv* env, jobject clazz)
{
    int rval = ::unloadWimaxDriver();

    return (jboolean)(rval == 0);
}

static jboolean com_htc_net_wimax_unloadWimaxDriver(JNIEnv* env, jobject clazz)
{
    int rval = ::unloadWimaxDriver();

    return (jboolean)(rval == 0);
}

// wimax daemon (sequansd)
static jboolean com_htc_net_wimax_startWimaxDaemon(JNIEnv* env, jobject clazz)
{
    int rval = ::startWimaxDaemon();

    return (jboolean)(rval == 0);
}

static jboolean com_htc_net_wimax_stopWimaxDaemon(JNIEnv* env, jobject clazz)
{
    int rval = ::stopWimaxDaemon();

    return (jboolean)(rval == 0);
}

// wimax properties
static jstring com_htc_net_wimax_getWimaxProp(JNIEnv* env, jobject clazz, jstring param1)
{
   const char *nativeString = (env)->GetStringUTFChars(param1, 0);
   char result_buffer[1024];
   char prop[30];
   strcpy(prop, nativeString);
   memset(result_buffer, 0, 1024);
   int result = ::getWimaxProp(prop, result_buffer);

   return env->NewStringUTF(result_buffer);
}

static jboolean com_htc_net_wimax_setWimaxProp(JNIEnv* env, jobject clazz, jstring param1, jstring param2)
{
    jboolean blnIsCopy;
    const char *nativeString1 = (env)->GetStringUTFChars(param1, 0);
    char prop[30];
    strcpy (prop, nativeString1);

    const char *nativeString2 = (env)->GetStringUTFChars(param2, 0);
    char val[30];
    strcpy (val, nativeString2);

    return (jboolean)(::setWimaxProp(prop, val) == 0);
}

// dhcp
static jboolean com_htc_net_wimax_stopDhcpWimaxDaemon(JNIEnv* env, jobject clazz)
{
    int rval = ::stopDhcpWimaxDaemon();

    return (jboolean)(rval == 0);
}

static jboolean com_htc_net_wimax_startDhcpWimaxDaemon(JNIEnv* env, jobject clazz)
{
    int rval = ::startDhcpWimaxDaemon();

    return (jboolean)(rval == 0);
}

static jboolean com_htc_net_wimax_doWimaxDhcpRelease(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::doWimaxDhcpRelease() == 0);
}

static jboolean com_htc_net_wimax_doWimaxDhcpRequest(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::doWimaxDhcpRequest() == 0);
}

static jboolean com_htc_net_wimax_terminateProcess(JNIEnv* env, jobject clazz, jstring param1)
{
   jboolean blnIsCopy;
   const char *strCIn = (env)->GetStringUTFChars(param1 , &blnIsCopy);
   char pid[10];
   strcpy (pid, strCIn);
   int rval = ::terminateProcess(pid);
 
   return (jboolean)(rval == 0);
}

// routing
static jboolean com_htc_net_wimax_addRouteToGateway(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::addRouteToGateway() == 0);
}

// Random ;x
static jboolean com_htc_net_wimax_testConnect(JNIEnv* env, jobject clazz)
{
    int rval = ::testConnect();
    return (jboolean)(rval == 0);
}

// interface sockets 
static jboolean com_htc_net_wimax_ThpIoctl(JNIEnv* env, jobject clazz, jint cmd, jint arg)
{
    return (jboolean)(::thpIoctl(cmd, arg) == 0);
}

static jboolean com_htc_net_wimax_ConfigInterface(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::wimaxConfigInterface() == 0);
}

// Debugging
static jboolean com_htc_net_wimax_wimaxDumpKmsg(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::wimaxDumpKmsg() == 0);
}

static jboolean com_htc_net_wimax_wimaxDumpLastKmsg(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::wimaxDumpLastKmsg() == 0);
}

static jboolean com_htc_net_wimax_wimaxDumpLogcat(JNIEnv* env, jobject clazz)
{
    return (jboolean)(::wimaxDumpLogcat() == 0);
}

// ----------------------------------------------------------------------------

/*
 * JNI registration.
 */
static JNINativeMethod gWimaxMethods[] = {
    /* name, signature, funcPtr */

    { "LoadDriver", "()Z",  (void *)com_htc_net_wimax_loadWimaxDriver },
    { "UnloadDriver", "()Z",  (void *)com_htc_net_wimax_unloadWimaxDriver },
    { "StartDriver", "()Z",  (void *)com_htc_net_wimax_startDriver },
    { "StopDriver", "()Z",  (void *)com_htc_net_wimax_stopDriver },
    { "StartDaemon", "()Z",  (void *)com_htc_net_wimax_startWimaxDaemon },
    { "StopDaemon", "()Z",  (void *)com_htc_net_wimax_stopWimaxDaemon },
    { "getWimaxProp", "(Ljava/lang/String;)Ljava/lang/String;",  (void *)com_htc_net_wimax_getWimaxProp },
    { "setWimaxProp", "(Ljava/lang/String;Ljava/lang/String;)Z",  (void *)com_htc_net_wimax_setWimaxProp },
    { "StopDhcpWimax", "()Z",  (void *)com_htc_net_wimax_stopDhcpWimaxDaemon },
    { "StartDhcpWimax", "()Z",  (void *)com_htc_net_wimax_startDhcpWimaxDaemon },
    { "DoWimaxDhcpRequest", "()Z",  (void *)com_htc_net_wimax_doWimaxDhcpRequest },
    { "DoWimaxDhcpRelease", "()Z",  (void *)com_htc_net_wimax_doWimaxDhcpRelease },
    { "TerminateProcess", "(Ljava/lang/String;)Z",  (void *)com_htc_net_wimax_terminateProcess },
    { "AddRouteToGateway", "()Z",  (void *)com_htc_net_wimax_addRouteToGateway },
    { "testConnect", "()Z",  (void *)com_htc_net_wimax_testConnect },
    { "ThpIoctl", "(IJ)Z",  (void *)com_htc_net_wimax_ThpIoctl },
    { "ConfigInterface", "()Z",  (void *)com_htc_net_wimax_ConfigInterface },
    { "DumpKmsg", "()Z",  (void *)com_htc_net_wimax_wimaxDumpKmsg },
    { "DumpLastKmsg", "()Z",  (void *)com_htc_net_wimax_wimaxDumpLastKmsg },
    { "DumpLogcat", "()Z",  (void *)com_htc_net_wimax_wimaxDumpLogcat },

};

int registerNatives(JNIEnv* env)
{
    jclass clazz;

    clazz = env->FindClass(WIMAX_PKG_NAME);
    if (clazz == NULL) {
        LOGE("Unable to find class " WIMAX_PKG_NAME);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gWimaxMethods, NELEM(gWimaxMethods)) < 0) {
        LOGE("RegisterNatives failed for %s", WIMAX_PKG_NAME);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint result = -1;

    LOGI("JNI_OnLoad");

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        goto bail;
    }

    if (!registerNatives(env)) {
        LOGE("ERROR: registerNatives failed");
        goto bail;
    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}

}
