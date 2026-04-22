// LibertyRecomp Android — :app module.
//
// Builds libLibertyRecomp.so through the repo-root CMakeLists.txt via
// `externalNativeBuild`, and wraps it into an APK with a single activity
// (com.libertyrecomp.LibertySDLActivity) that extends SDL3's SDLActivity.
//
// Three CMake arguments pin the build to the Android target:
//   LIBERTY_RECOMP_TARGET_PLATFORM=android
//   LIBERTY_RECOMP_ANDROID_RUNTIME_ASSETS=ON   (skip build-time payload check)
//   CMAKE_TOOLCHAIN_FILE=<repo>/toolchains/android.cmake
//
// The toolchain file chain-loads `android.toolchain.cmake` from the NDK bundle
// and forces LIBERTY_RECOMP_VULKAN=ON.

plugins {
    id("com.android.application")
}

android {
    namespace   = "com.libertyrecomp"
    compileSdk  = 35
    ndkVersion  = "27.2.12479018"

    defaultConfig {
        applicationId = "com.libertyrecomp"
        minSdk        = 26
        targetSdk     = 35
        versionCode   = 1
        versionName   = "0.1.0-dev"

        ndk {
            // 64-bit ARM is the default shipping ABI. x86_64 is opt-in via
            // `-PlibertyRecompEmulator=true` for Android emulator development.
            abiFilters += listOf("arm64-v8a")
            if (project.findProperty("libertyRecompEmulator") == "true") {
                abiFilters += "x86_64"
            }
        }

        externalNativeBuild {
            cmake {
                // Map to the same cache vars the CMakePresets set for the
                // `android-*` presets so Studio and preset-driven builds agree.
                arguments += listOf(
                    "-DLIBERTY_RECOMP_TARGET_PLATFORM=android",
                    "-DLIBERTY_RECOMP_ANDROID_RUNTIME_ASSETS=ON",
                    "-DANDROID_STL=c++_static",
                    "-DANDROID_PLATFORM=android-26",
                    "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
                )
                targets += "LibertyRecomp"
                cppFlags += "-std=c++23"
            }
        }
    }

    externalNativeBuild {
        cmake {
            // Reach the repo root to reuse the shared CMakeLists.txt.
            // os/android/app/build.gradle.kts → ../../../CMakeLists.txt
            path    = file("../../../CMakeLists.txt")
            version = "3.31.4"
        }
    }

    signingConfigs {
        // Custom release signing — override via gradle.properties or CLI:
        //   ./gradlew assembleRelease \
        //     -PlibertyRecompKeystore=/path/to/keystore.jks \
        //     -PlibertyRecompKeystorePassword=changeit \
        //     -PlibertyRecompKeyAlias=liberty \
        //     -PlibertyRecompKeyPassword=changeit
        create("release") {
            val ks = project.findProperty("libertyRecompKeystore")?.toString()
            if (ks != null) {
                storeFile     = file(ks)
                storePassword = project.findProperty("libertyRecompKeystorePassword")?.toString() ?: ""
                keyAlias      = project.findProperty("libertyRecompKeyAlias")?.toString() ?: ""
                keyPassword   = project.findProperty("libertyRecompKeyPassword")?.toString() ?: ""
            }
        }
    }

    buildTypes {
        debug {
            isDebuggable    = true
            isJniDebuggable = true
            isMinifyEnabled = false
        }
        release {
            isMinifyEnabled   = false
            isShrinkResources = false
            // Use the custom release signing config if a keystore was supplied,
            // otherwise fall back to the shared debug key.
            signingConfig = if (project.hasProperty("libertyRecompKeystore"))
                signingConfigs.getByName("release")
            else
                signingConfigs.getByName("debug")
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro",
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    packaging {
        jniLibs {
            // Preserve uncompressed .so placement so dlopen()/System.loadLibrary
            // can mmap the library directly instead of extracting at runtime.
            useLegacyPackaging = false
        }
        resources {
            excludes += listOf(
                "META-INF/LICENSE*",
                "META-INF/NOTICE*",
            )
        }
    }

    lint {
        checkReleaseBuilds = false
    }
}

dependencies {
    // No runtime AndroidX dependencies for the minimal shell.
    // Google Play Games v2 is opt-in for Phase-5 achievement wiring; keeping
    // it behind a build flag until we have a real Play Console entry.
    //
    // implementation("com.google.android.gms:play-services-games-v2:19.0.0")
}
