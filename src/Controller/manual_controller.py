import socket 
import time 

def readStringUntil(char: str) -> str: 
    string = ""
    while True:
        response = client.recv(1).decode()
        if response != char:
            string += response 
        else:
            break

    return string


host = "192.168.1.190"
port = 80
address = (host, port)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(address)

t1 = time.time()
client.send("2 100 10\n".encode())
response = readStringUntil("\n")
t2 = time.time()
print(response)
print(f"It took {t2 - t1} seconds to send the command and get a response")

# client.send("1 200 15\n".encode())
# response = readStringUntil("\n")
# print(response)

client.send("3 100 10\n".encode())
response = readStringUntil("\n")
print(response)

client.close()