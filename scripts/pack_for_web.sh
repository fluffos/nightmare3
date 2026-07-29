#!/usr/bin/env bash
#
# pack_for_web.sh -- pack this mudlib into a playable static browser bundle
# (WebAssembly FluffOS + web terminal), for GitHub Pages.
#
# Usage: scripts/pack_for_web.sh <driver_dir> <out_dir>
#   <driver_dir>  dir containing fluffos.js/fluffos.wasm/telnet.js/vendor/
#                 (an extracted fluffos release *-wasm.zip)
#   <out_dir>     output dir (created), ready to publish as a Pages site root
#
# Modeled on fluffos/mudlibs' scripts/pack_lib_for_web.sh, simplified for a
# single mudlib per repo (no multi-lib site index, so the driver files sit
# directly alongside index.html instead of a shared ../_driver/).
#
# gitignored runtime dirs (save/, log/ contents, etc.) are absent from a
# fresh checkout, and this mudlib's write_file()/save_object() calls throw
# (silently aborting the caller) if the target directory doesn't exist at
# all, even one that would hold a brand new file -- so every directory
# pattern named in .gitignore gets its SHAPE (not content) recreated in the
# staged copy before packing, not just the ones this repo already ships a
# tracked .gitignore placeholder inside.
#
# Requires: emscripten's file_packager (emsdk on PATH), python3.

set -euo pipefail
set -x   # trace every command -- helps whoever can actually read the raw
         # CI log; combined with the ::notice:: checkpoints below (which
         # surface as GitHub Actions *annotations*, readable through the
         # public API even without repo-admin auth) so a failure's
         # location is diagnosable either way. Strip once the pipeline
         # has proven stable across a few real runs.
note() { echo "::notice::pack_for_web: $*"; }

SELF_DIR=$(cd "$(dirname "$0")" && pwd)
REPO_ROOT=$(cd "$SELF_DIR/.." && pwd)

if [ $# -ne 2 ]; then
  echo "usage: $0 <driver_dir> <out_dir>" >&2
  exit 2
fi

DRIVER_DIR=$(cd "$1" && pwd)
OUT=$2
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)   # must be absolute: step 4 below cd's into $STAGE first,
                          # and a relative $OUT would then resolve against the
                          # stage dir instead of the caller's cwd (this is why
                          # CI, which passes a relative "site", failed at the
                          # file_packager step while every local repro --
                          # invoked with an absolute out dir -- passed clean)
[ -f "$DRIVER_DIR/fluffos.js" ] && [ -f "$DRIVER_DIR/fluffos.wasm" ] || {
  echo "error: driver not found in $DRIVER_DIR (need fluffos.js + fluffos.wasm)" >&2; exit 1; }
[ -f "$DRIVER_DIR/index.html" ] || { echo "error: $DRIVER_DIR/index.html not found" >&2; exit 1; }

FILE_PACKAGER=""
if command -v file_packager >/dev/null; then
  FILE_PACKAGER="file_packager"
else
  EMCC_PATH=$(command -v emcc || true)
  for c in "${EMCC_PATH:+$(dirname "$EMCC_PATH")/tools/file_packager.py}" \
           /usr/share/emscripten/tools/file_packager.py; do
    if [ -n "$c" ] && [ -f "$c" ]; then FILE_PACKAGER="python3 $c"; break; fi
  done
fi
[ -n "$FILE_PACKAGER" ] || { echo "error: emscripten file_packager not found (emsdk on PATH?)" >&2; exit 1; }
note "using FILE_PACKAGER=[$FILE_PACKAGER]"

STAGE=$(mktemp -d "${TMPDIR:-/tmp}/nm3pack.XXXXXX")
trap 'chmod -R u+w "$STAGE" >/dev/null 2>&1 || true; rm -rf "$STAGE" 2>/dev/null || true' EXIT
note "staging in $STAGE"

# --- 1. stage the mudlib tree (git-tracked files only: no local build/, save
#        data, or other untracked cruft leaks into the published bundle) ----
mkdir -p "$STAGE/mudlib"
FILE_COUNT=0
while IFS= read -r rel; do
  dst="$STAGE/mudlib/${rel#lib/}"
  mkdir -p "$(dirname "$dst")"
  cp "$REPO_ROOT/$rel" "$dst"
  FILE_COUNT=$((FILE_COUNT + 1))
done < <(cd "$REPO_ROOT" && git ls-files lib)
note "step 1 done: staged $FILE_COUNT tracked files"

# --- 2. recreate gitignored runtime-dir SHAPE (empty, no content) -----------
DIR_COUNT=0
while IFS= read -r pat; do
  rel=${pat#/lib/}; rel=${rel%/}
  mkdir -p "$STAGE/mudlib/$rel"
  DIR_COUNT=$((DIR_COUNT + 1))
done < <(grep -oE '^/lib/[A-Za-z0-9_./-]+/$' "$REPO_ROOT/.gitignore" 2>/dev/null || true)
mkdir -p "$STAGE/mudlib/log"
note "step 2 done: recreated $DIR_COUNT gitignored runtime dirs"

# --- 3. rewrite the config's mudlib directory to the in-image path and pack
#        it INSIDE the mudlib tree itself (config.fluffos.mount reads it as
#        a relative path from the chdir'd mount point, not a separate fetch)
python3 - "$REPO_ROOT/nm3.cfg" "$STAGE/mudlib/mudlib.cfg" <<'PYEOF'
import re, sys
src, dst = sys.argv[1], sys.argv[2]
text = open(src, encoding='utf-8', errors='surrogateescape').read()
text, n1 = re.subn(r'^(\s*mudlib directory\s*:\s*).*$', r'\g<1>/mudlib', text, flags=re.M)
if n1 != 1:
    sys.exit('error: expected exactly one "mudlib directory :" line, found %d' % n1)
open(dst, 'w', encoding='utf-8', errors='surrogateescape').write(text)
PYEOF
note "step 3 done: wrote mudlib.cfg"

# --- 4. pack with file_packager ---------------------------------------------
(cd "$STAGE" && $FILE_PACKAGER "$OUT/mudlib.data" \
    --preload "mudlib@/mudlib" \
    --js-output="$OUT/mudlib.js")
note "step 4 done: file_packager produced $OUT/mudlib.data + mudlib.js"

# --- 5. boot config + driver + page -----------------------------------------
cat > "$OUT/fluffos-boot.js" <<EOF
// Generated by scripts/pack_for_web.sh -- consumed by index.html.
window.FLUFFOS_BOOT = {
  mount: "/mudlib",
  config: "mudlib.cfg",
};
EOF
cp "$DRIVER_DIR/fluffos.js" "$DRIVER_DIR/fluffos.wasm" "$DRIVER_DIR/telnet.js" "$OUT/"
cp -r "$DRIVER_DIR/vendor" "$OUT/"
cp "$DRIVER_DIR/index.html" "$OUT/index.html"
note "step 5 done: driver + page copied into $OUT"

SIZE=$(du -sh "$OUT" | cut -f1)
echo "packed nightmare3 -> $OUT ($SIZE)"
