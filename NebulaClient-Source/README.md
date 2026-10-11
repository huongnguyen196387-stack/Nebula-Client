# Nebula Client — Android Native Scaffold

## Build configuration
- Android Gradle Plugin: 8.2.0
- Kotlin plugin: 1.9.0
- Gradle: 8.2 (the GitHub Actions workflow generates the Gradle Wrapper)
- JDK: 17
- Android SDK compile: 34
- NDK: 25.2.9519653
- CMake: 3.22.1
- Native ABI: `arm64-v8a`

## Current implementation status
- Android activity scaffold: present.
- JNI library load and status display: present in source.
- UI Choreographer callback-rate display: present; this is **not Minecraft FPS**.
- Standalone EGL pbuffer / GLES pixel smoke test: implemented in source, but not verified on a real device in this package-generation step.
- Minecraft Bedrock 1.26.50 ARM64 native hooks: **NOT IMPLEMENTED**.
- In-game renderer/menu: **NOT IMPLEMENTED**.
- Fullbright affecting the game: **NOT IMPLEMENTED**; current code stores only a boolean state.

## Build
Push this project to GitHub. The workflow at `.github/workflows/build.yml` attempts to build a debug APK and upload it as an Actions artifact. No successful build is claimed until the workflow completes successfully.
