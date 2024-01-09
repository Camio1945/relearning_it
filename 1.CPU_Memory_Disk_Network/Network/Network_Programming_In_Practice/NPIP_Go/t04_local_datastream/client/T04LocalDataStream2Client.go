package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
)

func main() {
	socket, err := net.Dial("unix", "/tmp/t04_local_data_stream_go.sock")
	panicIfError(err)
	defer socket.Close()
	fmt.Println("\nPlease input the message you want send to the server:")
	var recvLine []byte = make([]byte, 1024)
	for {
		sendLine, err := inputFromUser()
		panicIfError(err, "Error reading input")
		_, err = socket.Write(sendLine)
		panicIfError(err, "Write error")

		_, err = socket.Read(recvLine)
		panicIfError(err, "Read error")
		fmt.Printf("Received from client: %s\n", recvLine)
	}
}
func inputFromUser() ([]byte, error) {
	reader := bufio.NewReader(os.Stdin)
	sendLine, err := reader.ReadBytes('\n')
	panicIfError(err, "ReadBytes failed")
	return sendLine, nil
}
func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
