package main

import (
	"fmt"
	"net"
)

const HOST = "localhost"
const PORT = 9123

// This server accept only one client connection intentionally.
func main() {
	listener, err := net.Listen("tcp", fmt.Sprintf("%s:%d", HOST, PORT))
	defer listener.Close()
	panicIfError(err, "Error listening on port", PORT)
	fmt.Printf("\nServer is listening on port %d ...\n", PORT)

	conn, err := listener.Accept()
	defer conn.Close()
	panicIfError(err, "Error accepting connection")
	fmt.Println("A client connected.")

	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	panicIfError(err, "Error reading from client")
	fmt.Printf("Received from client: %s\n", string(buffer[:n]))

	_, err = conn.Write([]byte("Hello client, I am server.\n"))
	panicIfError(err, "Write data to client failed")
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
