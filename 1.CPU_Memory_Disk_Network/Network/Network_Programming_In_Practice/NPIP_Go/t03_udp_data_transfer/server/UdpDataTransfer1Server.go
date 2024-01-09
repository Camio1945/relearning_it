package main

import (
	"fmt"
	"net"
)

func main() {
	// Listen on UDP port 9123
	addr, err := net.ResolveUDPAddr("udp", ":9123")
	panicIfError(err, "net.ResolveUDPAddr failed")

	conn, err := net.ListenUDP("udp", addr)
	panicIfError(err, "net.ListenUDP failed")
	defer conn.Close()

	fmt.Println("\nServer is listening on port 9123 ...")

	// Receive a single message
	buffer := make([]byte, 1024)
	n, clientAddr, err := conn.ReadFromUDP(buffer)
	panicIfError(err, "conn.ReadFromUDP failed")
	fmt.Println("Received from client:", string(buffer[:n]))

	// Send response
	message := []byte("Hello client, I am server.")
	_, err = conn.WriteToUDP(message, clientAddr)
	panicIfError(err, "conn.WriteToUDP failed")
}

func panicIfError(err error, messages ...any) {
	if err != nil {
		panic(fmt.Sprint(messages, "\nerror: ", err))
	}
}
