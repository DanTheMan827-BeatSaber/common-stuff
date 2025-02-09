#include <jni.h>

#include <iostream>
#include <string>

#include "scotland2/shared/modloader.h"

namespace Helpers {
    /**
     * Retrieves the version of the current app bundle by using JNI to interact with the
     * Android application context, PackageManager, and PackageInfo.
     *
     * This function performs the following steps:
     * 1. Retrieves the application context via the ActivityThread class.
     * 2. Accesses the PackageManager from the application context.
     * 3. Retrieves the app's package name and then uses PackageManager to get package information.
     * 4. Extracts the version name from the PackageInfo and converts it to a C++ string.
     *
     * The version name is returned as a C++ std::string. If any error occurs during
     * the JNI calls, an empty string is returned.
     *
     * @return std::string The version of the current app bundle.
     */
    inline std::string getAppBundleVersion() {
        static std::string cachedVersion;

        if (!cachedVersion.empty()) {
            return cachedVersion;
        }

        JNIEnv* env;
        jint result = modloader_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

        if (result != JNI_OK) {
            std::cerr << "Failed to get JNI environment!" << std::endl;
            return "";
        }

        // Find the ActivityThread class
        jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
        if (activityThreadClass == nullptr) {
            std::cerr << "ActivityThread class not found!" << std::endl;
            return "";
        }

        // Create a global reference for the ActivityThread class
        jclass globalActivityThreadClass = (jclass) env->NewGlobalRef(activityThreadClass);
        jmethodID currentApplicationMethod = env->GetStaticMethodID(globalActivityThreadClass, "currentApplication", "()Landroid/app/Application;");
        if (currentApplicationMethod == nullptr) {
            std::cerr << "currentApplication method not found!" << std::endl;
            return "";
        }

        // Get the application context
        jobject applicationContext = env->CallStaticObjectMethod(globalActivityThreadClass, currentApplicationMethod);
        if (applicationContext == nullptr) {
            std::cerr << "Failed to get application context!" << std::endl;
            return "";
        }

        // Get the PackageManager from the application context
        jclass contextClass = env->GetObjectClass(applicationContext);
        jmethodID getPackageManagerMethod = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject packageManager = env->CallObjectMethod(applicationContext, getPackageManagerMethod);
        if (packageManager == nullptr) {
            std::cerr << "Failed to get PackageManager!" << std::endl;
            return "";
        }

        // Get the package name
        jmethodID getPackageNameMethod = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
        jstring packageName = (jstring) env->CallObjectMethod(applicationContext, getPackageNameMethod);
        if (packageName == nullptr) {
            std::cerr << "Failed to get package name!" << std::endl;
            return "";
        }

        // Get the PackageInfo from the PackageManager
        jclass packageManagerClass = env->GetObjectClass(packageManager);
        jmethodID getPackageInfoMethod =
            env->GetMethodID(packageManagerClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfoMethod, packageName, 0);
        if (packageInfo == nullptr) {
            std::cerr << "Failed to get PackageInfo!" << std::endl;
            return "";
        }

        // Get the version name from the PackageInfo
        jclass packageInfoClass = env->GetObjectClass(packageInfo);
        jfieldID versionNameField = env->GetFieldID(packageInfoClass, "versionName", "Ljava/lang/String;");
        jstring versionName = (jstring) env->GetObjectField(packageInfo, versionNameField);
        if (versionName == nullptr) {
            std::cerr << "Failed to get version name!" << std::endl;
            return "";
        }

        // Convert the version name to a C++ string
        char const* versionChars = env->GetStringUTFChars(versionName, nullptr);
        cachedVersion = std::string(versionChars);

        // Clean up JNI references
        env->ReleaseStringUTFChars(versionName, versionChars);
        env->DeleteLocalRef(versionName);

        return cachedVersion;
    }
}
