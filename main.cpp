#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <jni.h>
using namespace std;

void add_path(JNIEnv *env, const std::string& path);

int main() {
    JavaVM *vm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption* options = (JavaVMOption*) calloc(1, sizeof(JavaVMOption));

    stringstream ss;
    ss << "-cp /boot/system/lib/ghidra/Ghidra/Framework/Utility/lib/Utility.jar ";
    ss << "-Djava.system.class.loader=ghidra.GhidraClassLoader "; /*
    ss << "-Dfile.encoding=UTF8 ";
    ss << "-Duser.country=US ";
    ss << "-Duser.language=en ";
    ss << "-Duser.variant= ";
    ss << "-Dsun.java2d.opengl=false ";
    ss << "-Dhttps.protocols=TLS ";
    ss << "-Dghidra.cacerts=/boot/system/data/ssl/java/cacerts ";
    ss << "-Dcpu.core.limit= ";
    ss << "-Dcpu.core.override= ";
    ss << "-Dfont.size.override= ";
    ss << "-Dpython.console.encoding=UTF-8 ";
    ss << "-Xshare:off ";
    ss << "--illegal-access=permit ";
    ss << "-Dsun.java2d.pmoffscreen=false ";
    ss << "-Dsun.java2d.xrender=true ";
    ss << "-Dsun.java2d.uiScale=1 ";
    ss << "-Dawt.useSystemAAFontSettings=on";*/
    string args_string = ss.str();
    char args_string_c_copy[args_string.length()+1];
    args_string.copy(args_string_c_copy, args_string.length()+1, 0);
    args_string_c_copy[args_string.length()] = '\0';

    options[0].optionString = args_string_c_copy;

    vm_args.version = JNI_VERSION_10;
    vm_args.nOptions = 1;
    vm_args.ignoreUnrecognized = true;
    vm_args.options = options;

    jint res = JNI_CreateJavaVM(&vm, (void**)&env, &vm_args);
    if (res != JNI_OK) {
        cout << "JNI_Create() -> " << res << endl;
    }
    free(options);

    //add_path(env, "/boot/system/lib/ghidra/Ghidra/Framework/Utility/lib/Utility.jar");

    jclass clazz = env->FindClass("ghidra.GhidraLauncher");
    jthrowable mException = env->ExceptionOccurred();
    if (mException ) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        exit(1);
    }

    jstring gui_arg = env->NewStringUTF("-gui");
    if (gui_arg == NULL) {
        cout << "NewStringUTF8(); seems to be NULL" << endl;
    }

    jstring gui_class = env->NewStringUTF("ghidra.GhidraRun");
    if (gui_arg == NULL) {
        cout << "NewStringUTF8(); seems to be NULL" << endl;
    }

    jobjectArray args_array = (jobjectArray)env->NewObjectArray(2,env->FindClass("java/lang/String"),env->NewStringUTF(""));
    cout << "Created String[]" << endl;
    env->SetObjectArrayElement(args_array, 0, gui_class);
    cout << "Element 1 set for String[]" << endl;
    env->SetObjectArrayElement(args_array, 1, gui_arg);
    cout << "Element 1 set for String[]" << endl;

    jmethodID launch_main = env->GetMethodID(clazz, "main", "([Ljava/lang/String;)V");
    cout << "GetMethodID();" << endl;

    jobject result = env->CallObjectMethod(args_array, launch_main);
    cout << "CallOpjectMethod('gui', 'GhidraLauncher');" << endl;

    vm->DestroyJavaVM();
    return 0;
}

void add_path(JNIEnv *env, const std::string& path) {
    const std::string urlPath = "file:/" + path;
    jclass classLoaderCls = env->FindClass("java/lang/ClassLoader");
    jmethodID getSystemClassLoaderMethod = env->GetStaticMethodID(classLoaderCls, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoaderInstance = env->CallStaticObjectMethod(classLoaderCls, getSystemClassLoaderMethod);
    jclass urlClassLoaderCls = env->FindClass("java/net/URLClassLoader");
    jmethodID addUrlMethod = env->GetMethodID(urlClassLoaderCls, "addURL", "(Ljava/net/URL;)V");
    jclass urlCls = env->FindClass("java/net/URL");
    jmethodID urlConstructor = env->GetMethodID(urlCls, "<init>", "(Ljava/lang/String;)V");
    jobject urlInstance = env->NewObject(urlCls, urlConstructor, env->NewStringUTF(urlPath.c_str()));
    env->CallVoidMethod(classLoaderInstance, addUrlMethod, urlInstance);
#ifdef DEBUG
    cout << "Added " << urlPath << " to the classpath." << endl;
#endif
}