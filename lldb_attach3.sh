#!/bin/bash
# Launch app, wait for init to complete + a few frames, then attach LLDB to capture stuck thread
APP="./out/build/macos-release/LibertyRecomp/Liberty Recompiled.app/Contents/MacOS/Liberty Recompiled"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
LOG_DIR="out/build/macos-release/LibertyRecomp/run_logs"
APP_LOG="$LOG_DIR/app_${TIMESTAMP}.log"
LLDB_LOG="$LOG_DIR/lldb_${TIMESTAMP}.log"

mkdir -p "$LOG_DIR"

# Launch app in background
"$APP" > "$APP_LOG" 2>&1 &
APP_PID=$!
echo "App PID: $APP_PID"

# Wait 25s for init + 3 frames + blocking
sleep 25

# Check if still running
if ! kill -0 $APP_PID 2>/dev/null; then
  echo "App already exited"
  exit 1
fi

echo "Attaching LLDB at $TIMESTAMP..."

# LLDB script: get backtraces of all threads, focus on XThread ones
lldb -b -p $APP_PID -o "thread list" \
  -o "thread backtrace all" \
  > "$LLDB_LOG" 2>&1

echo "LLDB done, killing app..."
kill $APP_PID 2>/dev/null
wait $APP_PID 2>/dev/null

echo "LLDB log: $LLDB_LOG ($(wc -l < "$LLDB_LOG") lines)"
echo "App log: $APP_LOG ($(wc -l < "$APP_LOG") lines)"

# Show Main XThread backtrace
echo "--- Main XThread backtrace ---"
# Find "Main XThread" and show next 20 lines
awk '/Main XThread/{found=1} found{print; if(++n>25) exit}' "$LLDB_LOG"
