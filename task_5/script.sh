STR="vscode"
ps aux | grep -E "[0-50]+.*$STR" | grep -v grep | awk '{print $2}' | xargs kill -9
