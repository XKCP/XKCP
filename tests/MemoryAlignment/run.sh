#!/bin/bash
# Build an XKCP target with clang's or gcc's alignment sanitizer and run the
# misaligned-buffer test against it. Needs compiler-rt installed for clang or
# libubsan installed for gcc.
#
# Usage: run.sh <xkcp-tree> [target] [harness options]   (default target: generic64)
#
# Harness options go to misalign-test, e.g. --disableAVX512 on the x86-64 target.
#
# Pass/fail criteria:
#   - any "runtime error" from UBSan  -> misaligned lane access in the library
#   - nonzero harness exit, or no
#     final success line              -> wrong results on misaligned buffers
set -u
TREE=${1:?usage: run.sh <xkcp-tree> [target] [harness options]}
TARGET=${2:-generic64}
shift $(( $# < 2 ? $# : 2 ))
HERE=$(cd "$(dirname "$0")" && pwd)
COMPILER="${COMPILER:-clang}"
SAN="-fsanitize=alignment -g -O1"
# Keep running after a report (UBSan reports each source location once), and
# write the reports to the log, whatever the caller's settings.
export UBSAN_OPTIONS=halt_on_error=0

WORK=$(mktemp -d /tmp/misalign.XXXXXX)
trap 'rm -rf "$WORK"' EXIT              # kept on FAIL, see below
WRAP=$WORK/cc-ubsan
printf '#!/bin/sh\nexec %s %s "$@"\n' "$COMPILER" "$SAN" > "$WRAP"
chmod +x "$WRAP"

cd "$TREE" || exit 2
rm -rf bin/.build
make CC="$WRAP" "$TARGET/libXKCP.so" > /dev/null || { echo "library build failed"; exit 2; }

$COMPILER $SAN -I "bin/$TARGET/libXKCP.so.headers" "$HERE/misalign-test.c" "$HERE/misalign-xoofff.c" \
    "bin/$TARGET/libXKCP.so" -o "$WORK/misalign-test" || exit 2

LOG=$WORK/misalign-test.log
LD_LIBRARY_PATH="$TREE/bin/$TARGET" "$WORK/misalign-test" "$@" > "$LOG" 2>&1
RC=$?
ERRS=$(grep -c "runtime error" "$LOG")
DONE=$(grep -c "^All misaligned-buffer results match" "$LOG")

echo "== $TARGET $* in $TREE =="
grep -E '^(Keccak-p|Xoodoo)' "$LOG" | sed 's/^/   /'
echo "   UBSan alignment errors: $ERRS"
echo "   result mismatches:      $(grep -c MISMATCH "$LOG") (harness rc=$RC)"
grep "runtime error" "$LOG" | sed 's/.*runtime error/   e.g./' | sort -u | head -3
FAIL=0
[ "$ERRS" -eq 0 ] && [ "$RC" -eq 0 ] && [ "$DONE" -eq 1 ] || FAIL=1

if [ "$FAIL" -eq 0 ]; then
    echo "   PASS"
    exit 0
else
    trap - EXIT
    echo "   FAIL (logs in $WORK); last lines of the harness log:"
    tail -n 20 "$LOG" | sed 's/^/   | /'
    exit 1
fi
