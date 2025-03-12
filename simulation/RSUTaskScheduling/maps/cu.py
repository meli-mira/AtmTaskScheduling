import socket
import threading

HOST = "127.0.0.1" 
PORT = 5000  

rsu_tasks = {}  

def handle_rsu_connection(conn, addr):
    print(f"New connection {addr}")
    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            message = data.decode("utf-8")
            print(f"[{addr}] {message}")
            
            rsu_id, processed_tasks = message.split(":")
            processed_tasks = int(processed_tasks)
            
            if rsu_id in rsu_tasks:
                rsu_tasks[rsu_id] += processed_tasks
            else:
                rsu_tasks[rsu_id] = processed_tasks
            
            print(f"Acc: {rsu_tasks}")
            conn.sendall(b"ACK")



def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((HOST, PORT))
        server_socket.listen()
        print(f"Waiting for connections on {HOST}:{PORT}...")
        
        while True:
            conn, addr = server_socket.accept()
            thread = threading.Thread(target=handle_rsu_connection, args=(conn, addr))
            thread.start()

if __name__ == "__main__":
    start_server()