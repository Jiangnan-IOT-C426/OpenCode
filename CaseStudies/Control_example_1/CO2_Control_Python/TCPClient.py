from socket import *


def Select_Parameter(parameter):
    out = parameter
    tcp_client_socket = socket(AF_INET, SOCK_STREAM)
    tcp_client_socket.connect(("127.0.0.1", 8086))
    meg = "Select_"+out
    tcp_client_socket.send(meg.encode())
    while True:
        receive = tcp_client_socket.recv(1024)
        if len(receive) > 0:
            break
    if out == "Tem":
        result = (receive[3] << 8 | receive[4]) / 10
    elif out == "Hum":
        result = (receive[5] << 8 | receive[6]) / 10
    elif out == "Lig":
        result = receive[7] << 8 | receive[8]
    elif out == "Dio":
        result = receive[9] << 8 | receive[10]
    elif out == "Flour":
        result = receive[11] << 8 | receive[12]
    tcp_client_socket.close()
    return result


def Control_Operator(parameter):
    out = parameter
    meg = "Control_" + out
    tcp_client_socket = socket(AF_INET, SOCK_STREAM)
    tcp_client_socket.connect(("127.0.0.1", 8086))
    tcp_client_socket.send(meg.encode())
    tcp_client_socket.close()






