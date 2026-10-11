#!/usr/bin/env bash
# Kiểm tra ABI và symbol JNI của libnebulaclient.so trong APK.
# Usage: scripts/verify_native.sh <apk>
# Env:   READELF, NM (mặc định readelf, nm; CI dùng llvm-readelf/llvm-nm của NDK)
set -uo pipefail

APK="${1:?usage: verify_native.sh <apk>}"
READELF="${READELF:-readelf}"
NM="${NM:-nm}"
EXPECTED_LIB="lib/arm64-v8a/libnebulaclient.so"
REQUIRED_EXPORTS=(
  Java_com_nebula_MainActivity_nativeInitClient
  Java_com_nebula_MainActivity_nativeGetStatus
  Java_com_nebula_MainActivity_nativeRunOpenGlesTest
)

failures=0
ok()   { echo "OK:   $*"; }
warn() { echo "WARN: $*"; }
fail() { echo "FAIL: $*"; failures=$((failures + 1)); }

echo "APK: $APK"
echo "readelf: $READELF | nm: $NM"
[ -f "$APK" ] || { fail "không tìm thấy APK"; exit 1; }

mapfile -t libs < <(unzip -Z1 "$APK" | grep -E '^lib/.+\.so$' || true)
echo "Thư viện native trong APK: ${libs[*]:-(không có)}"
if [ "${#libs[@]}" -eq 1 ] && [ "${libs[0]}" = "$EXPECTED_LIB" ]; then
  ok "chỉ có $EXPECTED_LIB (không có ABI khác)"
else
  fail "kỳ vọng đúng một thư viện $EXPECTED_LIB"
  [ "${#libs[@]}" -eq 0 ] && exit 1
fi

tmp="$(mktemp -d)"; trap 'rm -rf "$tmp"' EXIT
unzip -q -o "$APK" "$EXPECTED_LIB" -d "$tmp" || { fail "không giải nén được $EXPECTED_LIB"; exit 1; }
so="$tmp/$EXPECTED_LIB"

hdr="$("$READELF" -h "$so" 2>&1)"
echo "$hdr" | grep -Eq 'Class:\s+ELF64' && ok "ELF64" || fail "không phải ELF64"
echo "$hdr" | grep -Eq 'Machine:\s+(AArch64|ARM aarch64)' && ok "Machine: AArch64" || fail "Machine không phải AArch64"

mapfile -t exports < <("$NM" -D --defined-only "$so" 2>/dev/null | awk '{print $NF}')
echo "Số symbol động được export: ${#exports[@]}"
for sym in "${REQUIRED_EXPORTS[@]}"; do
  printf '%s\n' "${exports[@]}" | grep -Fxq "$sym" && ok "export $sym" || fail "thiếu export $sym"
done

mapfile -t extra < <(printf '%s\n' "${exports[@]}" | grep -Ev '^(Java_com_nebula_|JNI_OnLoad$|JNI_OnUnload$)' || true)
if [ "${#extra[@]}" -gt 0 ]; then
  warn "${#extra[@]} symbol export ngoài JNI (hiển thị tối đa 15): ${extra[*]:0:15}"
else
  ok "không có symbol export ngoài JNI"
fi

echo "NEEDED:"; "$READELF" -d "$so" 2>/dev/null | grep NEEDED || true

if [ "$failures" -gt 0 ]; then echo "KẾT QUẢ: $failures lỗi"; exit 1; fi
echo "KẾT QUẢ: đạt"
