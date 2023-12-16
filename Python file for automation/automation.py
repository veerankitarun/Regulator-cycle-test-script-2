import serial
import serial.tools.list_ports
import time

def find_arduino_port():
    arduino_ports = [
        p.device
        for p in serial.tools.list_ports.comports()
        if 'Arduino' in p.description
    ]
    if arduino_ports:
        return arduino_ports[0]
    return None

def send_command(command):
    ser.write(command.encode('utf-8'))
    response = ser.readline().decode('utf-8').strip()
    return response

def read_config():
    try:
        with open('config.txt', 'r') as config_file:
            cycles = int(config_file.readline().split(':')[1].strip())
        return cycles
    except FileNotFoundError:
        print("Config file not found. Make sure to create a config.txt file.")
        return None

def main():
    try:
        # Find Arduino port
        arduino_port = find_arduino_port()
        if arduino_port:
            print(f"Found Arduino on port: {arduino_port}")
        else:
            print("Arduino not found. Check the connection.")
            return

        # Connect to Arduino
        global ser
        ser = serial.Serial(arduino_port, 9600)

        # Define the number of cycles (read from config.txt)
        cycles = read_config()
        if cycles is None:
            return

        for cycle in range(cycles):
            # Step 1
            response = send_command("CMD=0,0")
            if response == "OK":
                print("Step 1 completed successfully")
            time.sleep(5)

            # Step 2
            response = send_command("CMD=3,1")
            if response == "OK":
                print("Step 2 completed successfully")
            time.sleep(120)

            # Step 3
            response = send_command("CMD=0,0")
            if response == "OK":
                print("Step 3 completed successfully")
            response = send_command("CMD=4,1")
            if response == "OK":
                print("Step 3 continued successfully")
            time.sleep(180)

            # Step 4
            response = send_command("CMD=0,0")
            if response == "OK":
                print("Step 4 completed successfully")

        print(f"All cycles completed successfully")

    except Exception as e:
        print(f"Error: {e}")

    finally:
        # Close the serial connection
        if ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
