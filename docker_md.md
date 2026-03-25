### python logs

    *** Now that the API is sending jobs, you should check your worker logs to see if it's actually processing the image: ***
    docker compose logs worker -f

    docker compose logs worker

### api logs

    docker compose logs api

### Testing with Curl

    curl -X POST "http://localhost:8000/generate" -H "Content-Type: application/json" -d '{
  "input": "/data/uploads/input.jpg",
  "output": "/data/outputs/out.png",
  "cols": 6,
  "rows": 2
}'

### Steps for your Worker

    Now that the API is sending jobs, you should check your worker logs to see if it's actually processing the image:
    docker compose logs worker -f

### Standard COMPOSE UP/DOWN ( from anywhere )

    The correct syntax to build and start your containers in detached mode from an outside directory is:

    docker compose -f ./infra/docker-compose.yml up --build -d

    docker compose -f ./infra/docker-compose.yml down
