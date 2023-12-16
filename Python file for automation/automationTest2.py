import serial
import time
import json

def main():
    # Load configurations from config.txt
    try:
        with open("config.txt", "r") as config_file:
            config = json.load(config_file)
    except Exception as e:
        print(f"Error reading config file: {e}")
        return

    # Establish serial connection with Arduino
    ser = serial.Serial(port=config.get('com_port', 'COM6'), baudrate=9600, timeout=1)
    time.sleep(2)  # Allow time for the serial connection to stabilize

    try:
        # Perform specified number of cycles
        for cycle in range(config.get('cycles', 1)):
            # Send command "CMD=0,0" and wait for response
            send_command(ser, "CMD=0,0")

            # Wait for 5 seconds
            time.sleep(5)

            # Send command "CMD=3,1" and wait for response
            send_command(ser, "CMD=3,1")

            # Wait for 2 minutes 120
            time.sleep(10)

            # Send command "CMD=0,0" and wait for response
            send_command(ser, "CMD=0,0")

            # Send command "CMD=4,1" and wait for response
            send_command(ser, "CMD=4,1")

            # Wait for 3 minutes 180
            time.sleep(10)

            # Send command "CMD=0,0" and wait for response
            send_command(ser, "CMD=0,0")
        print("")
        print("-------------------------------------------")
        print("")
        print(f"\033[92mCompleted {cycle} Cycles!\033[0m")
        print("")
        print("-------------------------------------------")

        # Close the serial connection
        ser.close()
    except Exception as e:
        print(f"Error: {e}")
        ser.close()

def send_command(ser, command):
    # Send command to Arduino
    ser.write((command + '\r\n').encode('utf-8'))

    # Wait for a short time to allow Arduino to process the command
    time.sleep(0.5)

    # Read and display the response from Arduino
    response = ser.readline().decode('utf-8').strip()
    print(f"Command: {command}, Response: {response}")

if __name__ == "__main__":
    main()

