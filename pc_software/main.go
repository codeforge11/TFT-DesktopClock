package main

import (
	"time"

	"go.bug.st/serial"
)

func main() {
	port, err := serial.Open("COM3", &serial.Mode{BaudRate: 9600}) //Set the right serial port for your computer

	if err != nil {
		panic(err)
	}
	defer port.Close()

	for i := 0; i < 3; i++ {
		currentTime := time.Now().Format("15:04")
		message := currentTime + "\n"

		_, err := port.Write([]byte(message))
		if err != nil {
			panic(err)
		}

		time.Sleep(5 * time.Second)
	}
}
