#include <jni.h>

#include <concepts>
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

#if __has_include("scotland2/shared/modloader.h")
#include "scotland2/shared/modloader.h"
#endif

#include "../../Concepts.hpp"

namespace DanTheMan827::Helpers::Package {
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
    template <Concepts::Logger L = void>
    inline std::string getAppBundleVersion(L* Logger = nullptr) {
        static std::string cachedVersion;

        if (!cachedVersion.empty()) {
            return cachedVersion;
        }

        JNIEnv* env;
        jint result = modloader_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

        if (result != JNI_OK) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get JNI environment!");
            }
            return "";
        }

        // Find the ActivityThread class
        jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
        if (activityThreadClass == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("ActivityThread class not found!");
            }
            return "";
        }

        // Create a global reference for the ActivityThread class
        jclass globalActivityThreadClass = (jclass) env->NewGlobalRef(activityThreadClass);
        jmethodID currentApplicationMethod = env->GetStaticMethodID(globalActivityThreadClass, "currentApplication", "()Landroid/app/Application;");
        if (currentApplicationMethod == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("currentApplication method not found!");
            }
            return "";
        }

        // Get the application context
        jobject applicationContext = env->CallStaticObjectMethod(globalActivityThreadClass, currentApplicationMethod);
        if (applicationContext == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get application context!");
            }
            return "";
        }

        // Get the PackageManager from the application context
        jclass contextClass = env->GetObjectClass(applicationContext);
        jmethodID getPackageManagerMethod = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject packageManager = env->CallObjectMethod(applicationContext, getPackageManagerMethod);
        if (packageManager == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get PackageManager!");
            }
            return "";
        }

        // Get the package name
        jmethodID getPackageNameMethod = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
        jstring packageName = (jstring) env->CallObjectMethod(applicationContext, getPackageNameMethod);
        if (packageName == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get package name!");
            }
            return "";
        }

        // Get the PackageInfo from the PackageManager
        jclass packageManagerClass = env->GetObjectClass(packageManager);
        jmethodID getPackageInfoMethod =
            env->GetMethodID(packageManagerClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        jobject packageInfo = env->CallObjectMethod(packageManager, getPackageInfoMethod, packageName, 0);
        if (packageInfo == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get PackageInfo!");
            }
            return "";
        }

        // Get the version name from the PackageInfo
        jclass packageInfoClass = env->GetObjectClass(packageInfo);
        jfieldID versionNameField = env->GetFieldID(packageInfoClass, "versionName", "Ljava/lang/String;");
        jstring versionName = (jstring) env->GetObjectField(packageInfo, versionNameField);
        if (versionName == nullptr) {
            if constexpr (!std::is_same<L, void>::value) {
                Logger->error("Failed to get version name!");
            }
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
}  // namespace DanTheMan827::Helpers::Package
