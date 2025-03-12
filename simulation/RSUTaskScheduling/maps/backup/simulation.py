import traci
import requests
import json
import os

SUMO_CMD = ["sumo-gui", "-c", "bucharest.sumocfg"]  


#6969 7979 8989 9999 10099 11099 12099 13099
RSU_LOCATIONS = {
    "RSU_1": {"coords": (252, 185), "url": "http://localhost:6969/task"},
    "RSU_2": {"coords": (225, 151), "url": "http://localhost:7979/task"},
    "RSU_3": {"coords": (210, 415), "url": "http://localhost:8989/task"},
    "RSU_4": {"coords": (245, 390), "url": "http://localhost:9999/task"},
    "RSU_5": {"coords": (270, 50), "url": "http://localhost:10099/task"},
    "RSU_6": {"coords": (190, 929), "url": "http://localhost:11099/task"},
    "RSU_7": {"coords": (315, 450), "url": "http://localhost:12099/task"},
    "RSU_8": {"coords": (10, 195), "url": "http://localhost:13099/task"},
    
    
}


DISTANCE_THRESHOLD = 200  

def euclidean_distance(x1, y1, x2, y2):
    return ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5

traci.start(SUMO_CMD)

while traci.simulation.getMinExpectedNumber() > 0:
    traci.simulationStep()  

    vehicles = traci.vehicle.getIDList()
    
    for veh_id in vehicles:
        x, y = traci.vehicle.getPosition(veh_id)

        for rsu_id, rsu_data in RSU_LOCATIONS.items():
            rsu_x, rsu_y = rsu_data["coords"]
            distance = euclidean_distance(x, y, rsu_x, rsu_y)

            if distance < DISTANCE_THRESHOLD:
                task_data = {
                    "name": f"task_{veh_id}",
                    "description": "Generated from SUMO",
                    "request": "2025-04-08",
                    "deadline": "2025-04-20",
                    "duration": "5",
                    "priority": "12",
                    "tasktype": "INTERVAL_BASED",
                    "node_id": "1"
                }

                try:
                    response = requests.post(rsu_data["url"], json=task_data)
                    if response.status_code == 201:
                        print(f"Task sent to {rsu_id}: {task_data['name']}")
                    else:
                        print(f"Failed to send task to {rsu_id}: {response.status_code}")
                except requests.exceptions.RequestException as e:
                    print(f"Error sending task to {rsu_id}: {e}")

traci.close()
