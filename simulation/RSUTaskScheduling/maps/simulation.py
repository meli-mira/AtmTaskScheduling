import traci
import requests
import random
import datetime
from enum import Enum

SUMO_CMD = ["sumo-gui", "-c", "bucharest.sumocfg"]

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

class TaskSubtype(Enum):
    TYPE1 = "TYPE1"
    TYPE2 = "TYPE2"
    TYPE3 = "TYPE3"
    TYPE4 = "TYPE4"
    TYPE5 = "TYPE5"
    TYPE6 = "TYPE6"
    TYPE7 = "TYPE7"
    TYPE8 = "TYPE8"
    TYPE9 = "TYPE9"
    TYPE10 = "TYPE10"

def euclidean_distance(x1, y1, x2, y2):
    return ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5

def generate_task(veh_id):
    today = datetime.date.today()
    request_date = today + datetime.timedelta(days=random.randint(0, 5))  
    deadline_date = request_date + datetime.timedelta(days=random.randint(10, 30))  
    duration = random.randint(1, 10)  
    priority = random.randint(10, 20)  
    task_subtype = random.choice(list(TaskSubtype)).value  # Select a random task subtype

    return {
        "name": f"task_{veh_id}_{random.randint(1000, 9999)}",
        "description": "Generated from SUMO simulation",
        "request": str(request_date),
        "deadline": str(deadline_date),
        "duration": str(duration),
        "priority": str(priority),
        "tasktype": "INTERVAL_BASED",
        "tasksubtype": task_subtype,  # Random subtype from ENUM
        "node_id": "1"
    }

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
                task_data = generate_task(veh_id)

                try:
                    response = requests.post(rsu_data["url"], json=task_data)
                    if response.status_code == 201:
                        print(f"Task sent to {rsu_id}: {task_data}")
                    else:
                        print(f"Failed to send task to {rsu_id}: HTTP {response.status_code}")
                except requests.exceptions.RequestException as e:
                    print(f"Error sending task to {rsu_id}: {e}")

traci.close()
