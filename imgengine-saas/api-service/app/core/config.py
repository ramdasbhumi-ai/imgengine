# backend/core/config.py 

import os

DATABASE_URL = os.getenv(
    "DATABASE_URL", "postgresql://imgengine:imgengine@db:5432/imgengine"
)

API_GENERATE_RATE_LIMIT = os.getenv("API_GENERATE_RATE_LIMIT", "5/minute")
