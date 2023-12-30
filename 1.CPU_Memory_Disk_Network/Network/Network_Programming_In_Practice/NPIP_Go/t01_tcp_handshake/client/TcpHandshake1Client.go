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
	println("Connected to server.")
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
