package main

import (
	"fmt"
	"net"
)

const HOST = "localhost"
const PORT = 9123

func main() {
	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%d", HOST, PORT))
	defer conn.Close()
	panicIfError(err, "Failed to connect to server")
	println("\nConnected to server.")

	_, err = conn.Write([]byte("Hello server, I am client.\n"))
	panicIfError(err, "Write data to server failed")

	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	panicIfError(err, "Error reading from server")
	fmt.Printf("Received from server: %s\n", string(buffer[:n]))
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
