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
	fmt.Printf("Server is listening on port %d ...\n", PORT)
	conn, err := listener.Accept()
	defer conn.Close()
	panicIfError(err, "Error accepting connection")
	fmt.Println("A client connected.")
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
