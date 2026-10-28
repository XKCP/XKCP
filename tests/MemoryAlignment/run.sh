#!/bin/bash
# Build an XKCP target with clang's or gcc's alignment sanitizer and run the
# misaligned-buffer test against it. Needs compiler-rt installed for clang or
# libubsan installed for gcc.
#
# Usage: run.sh <xkcp-tree> [target] [harness options]   (default target: generic64)
#
# Harness options go to misalign-test, e.g. --disableAVX512 on the x86-64 target.
# If RUNNER is set, it is the command that runs the test programs, for example
# an emulator: RUNNER="sde64 -icx --" run.sh . x86-64
# If WITH_UNITTESTS=1, the unit tests (UnitTests -a, with the same options) are
# also built with the sanitizer and run. Many of them use misaligned buffers.
#
# Pass/fail criteria:
#   - any "runtime error" from UBSan  -> misaligned lane access in the library
#   - nonzero harness exit, or no
#     final success line              -> wrong results on misaligned buffers
#   - with WITH_UNITTESTS=1: any "runtime error" or a nonzero exit of UnitTests
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
# RUNNER is split into words on purpose.
# shellcheck disable=SC2086
LD_LIBRARY_PATH="$TREE/bin/$TARGET" ${RUNNER:-} "$WORK/misalign-test" "$@" > "$LOG" 2>&1
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

if [ "${WITH_UNITTESTS:-0}" = 1 ]; then
    make CC="$WRAP" "$TARGET/UnitTests" > /dev/null || { echo "unit test build failed"; exit 2; }
    ULOG=$WORK/UnitTests.log
    # shellcheck disable=SC2086
    ${RUNNER:-} "bin/$TARGET/UnitTests" -a "$@" > "$ULOG" 2>&1
    URC=$?
    UERRS=$(grep -c "runtime error" "$ULOG")
    echo "   UnitTests -a: UBSan alignment errors: $UERRS (rc=$URC)"
    grep "runtime error" "$ULOG" | sed 's/.*runtime error/   e.g./' | sort -u | head -3
    if [ "$UERRS" -ne 0 ] || [ "$URC" -ne 0 ]; then
        FAIL=1
        echo "   last lines of the UnitTests log:"
        tail -n 20 "$ULOG" | sed 's/^/   | /'
    fi
fi

if [ "$FAIL" -eq 0 ]; then
    echo "   PASS"
    exit 0
else
    trap - EXIT
    echo "   FAIL (logs in $WORK); last lines of the harness log:"
    tail -n 20 "$LOG" | sed 's/^/   | /'
    exit 1
fi
