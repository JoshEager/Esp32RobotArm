import socket

board_host = "192.168.1.189"
board_port = 80
board_address = (board_host, board_port)

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(board_address)

data = client_socket.recv(64);
print(data.decode("utf-8"))

client_socket.send("Hello from the client".encode())

client_socket.close()
print("Disconnected from server")


