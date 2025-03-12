#!/bin/bash

echo "Building the RSU Docker image..."
docker build -t rsu_image -f Dockerfile.rsu .

RSU_PORTS=(6969 7979 8989 9999 10099 11099 12099 13099)

for i in "${!RSU_PORTS[@]}"; do
    RSU_ID=$((i + 1))
    PORT=${RSU_PORTS[$i]}
    echo "Deploying RSU_$RSU_ID on port $PORT..."
    
    docker run -d -p $PORT:6969 --name rsu_$RSU_ID rsu_image

    sleep 5

    echo "Configuring RSU_$RSU_ID..."
    ./configure_container.sh http://localhost:$PORT
done

echo "All RSUs have been deployed and configured successfully!"
