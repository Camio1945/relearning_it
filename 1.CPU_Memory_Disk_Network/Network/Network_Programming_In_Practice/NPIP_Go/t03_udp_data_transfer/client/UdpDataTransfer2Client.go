package main

import (
	"fmt"
	"net"
)

func main() {
	addr, err := net.ResolveUDPAddr("udp", "localhost:9123")
	panicIfError(err, "net.ResolveUDPAddr failed")

	conn, err := net.DialUDP("udp", nil, addr)
	panicIfError(err, "net.DialUDP failed")
	defer conn.Close()
	//conn.SetDeadline(time.Now().Add(time.Second))

	// Send message
	message := []byte("Hello server, I am client.")
	_, err = conn.Write(message)
	panicIfError(err, "conn.Write failed")

	// Receive response
	buffer := make([]byte, 1024)
	n, _, err := conn.ReadFromUDP(buffer)
	panicIfError(err, "conn.ReadFromUDP failed")
	fmt.Println("\nReceived from server:", string(buffer[:n]))
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
