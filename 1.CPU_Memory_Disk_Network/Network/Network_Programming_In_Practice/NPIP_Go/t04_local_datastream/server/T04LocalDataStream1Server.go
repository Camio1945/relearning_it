package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	os.Remove("/tmp/t04_local_data_stream_go.sock")
	listenFd, err := net.Listen("unix", "/tmp/t04_local_data_stream_go.sock")
	panicIfError(err, "Listen failed")
	defer listenFd.Close()

	fmt.Println("\nServer is listening on /tmp/t04_local_data_stream_go.sock ...")

	conn, err := listenFd.Accept()
	panicIfError(err, "Accept failed")
	defer conn.Close()

	message := make([]byte, 1024)
	for {
		n, err := conn.Read(message)
		panicIfError(err, "Read failed")
		message[n] = 0
		fmt.Printf("Received from client: %s\n", message)

		sendLine := fmt.Sprintf("Hi, %s", message)
		_, err = conn.Write([]byte(sendLine))
		panicIfError(err)
	}
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
