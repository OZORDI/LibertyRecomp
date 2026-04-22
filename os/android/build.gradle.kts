// LibertyRecomp Android — root build file.
//
// Declares the Android Gradle plugin version used by the :app module.
// Keep this in sync with the Kotlin/AGP matrix at
// https://developer.android.com/build/releases/gradle-plugin.
//
// AGP 8.7.x ↔ Gradle 8.9+   ↔ JDK 17+
// AGP 8.6.x ↔ Gradle 8.7+   ↔ JDK 17+

plugins {
    id("com.android.application") version "8.7.3" apply false
}
