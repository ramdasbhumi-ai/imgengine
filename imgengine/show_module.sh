# #!/bin/bash

# MODULE_PATH=$1

# if [ -z "$MODULE_PATH" ]; then
#   echo "Usage: ./show_module.sh <module_path>"
#   exit 1
# fi

# echo "🔍 Showing module: $MODULE_PATH"
# echo "----------------------------------------"

# for f in $(find "$MODULE_PATH" -type f \( -name "*.h" -o -name "*.c" \) | sort); do
#   echo ""
#   echo "===== $f ====="
#   cat "$f"
# done



#!/bin/bash

MODULE_PATH=$1

if [ -z "$MODULE_PATH" ]; then
  echo "Usage: ./show_module.sh <module_path>"
  exit 1
fi

echo "========================================"
echo "📦 MODULE: $MODULE_PATH"
echo "========================================"

find "$MODULE_PATH" -type f \( -name "*.h" -o -name "*.c" \) | sort | while read -r f; do
  echo ""
  echo "----------------------------------------"
  echo "📄 FILE: $f"
  echo "----------------------------------------"
  cat "$f"
done

# chmod +x show_module.sh
# 🚀 Usage
# API module:
# ./show_module.sh imgengine/api/v1
# Core:
# ./show_module.sh imgengine/src/core
# Memory:
# ./show_module.sh imgengine/memory