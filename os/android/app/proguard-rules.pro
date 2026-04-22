# Keep SDL and LibertyRecomp Java↔native plumbing.
# Anything called via JNI must survive shrinking.

-keep class com.libertyrecomp.** { *; }
-keep class org.libsdl.app.** { *; }
-keepclasseswithmembers class * {
    native <methods>;
}

# Preserve annotations used by Android framework reflection.
-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod
