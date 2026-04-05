i wanna do a simple scp (secure copy)
just able to transfer files from machine A to machine B

for now, im just gonna do simple file transfers on local machine over tcp sockets
it would be wiser to do ipc but tedious to refactor later

goals:
1. simple tcp transfers locally (practically just ipc transfers)
2. transfer over ip
3. encrypt bytes sent/received