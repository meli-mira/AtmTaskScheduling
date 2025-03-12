#!/bin/bash

if [ -z "$1" ]; then
    echo " Error: Please provide the RSU API URL as an argument."
    echo "Usage: ./configure_container.sh http://localhost:6969"
    exit 1
fi

RSU_URL="$1"

send_request() {
    local endpoint=$1
    local data=$2

    response=$(curl -s -o /dev/null -w "%{http_code}" -X POST "$RSU_URL/$endpoint" \
        -H "Content-Type: application/json" \
        -d "$data")

    if [ "$response" -eq 200 ] || [ "$response" -eq 201 ]; then
        echo " Successfully added $endpoint"
    else
        echo " Failed to add $endpoint. HTTP Status: $response"
    fi
}



send_request "node" '{
    "name": "RSU",
    "capacity": "2",
    "minpriority": "10",
    "maxpriority": "20",
    "parent_node_id": "1"
}'

for i in {1..100}; do
    send_request "resource" "{
        \"name\": \"RESOURCE_TYPE$i\",
        \"capacity\": \"$(( (i % 5) + 1 ))\"
    }"
done

echo " Database population completed!"
