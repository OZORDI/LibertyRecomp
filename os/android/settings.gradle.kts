// LibertyRecomp Android — root settings.
//
// Single-module Gradle project. The `app` module wraps the native
// libLibertyRecomp.so (built by CMake via `externalNativeBuild`) in an APK.

pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "LibertyRecomp"
include(":app")
