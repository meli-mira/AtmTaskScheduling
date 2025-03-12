#!/bin/bash

RSU_CONTAINERS=$(docker ps -a --filter "name=rsu_" --format "{{.Names}}")

echo "Stopping and removing RSU containers..."
for container in $RSU_CONTAINERS; do
    echo "Stopping $container..."
    docker stop $container
    echo "Removing $container..."
    docker rm $container
done

echo "Removing RSU Docker image..."
docker rmi rsu_image

echo "Removing unused Docker resources..."
docker system prune -f

echo "Cleanup completed!"
