# ===================================================================
# VERTEX-X1 HARDWARE JOINT CALIBRATION UTILITY
# ===================================================================
import serial
import time

def run_calibration():
    # استبدل COM3 بـ البورت الحقيقي للـ ESP32 على جهازك
    port = "COM3" 
    baudrate = 11200
    
    try:
        esp32 = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2) # انتظار إعادة تشغيل البوردة بعد الفتح
        print(f"[+] Calibration link established on {port}")
        print("[+] Instructions: Enter Joint ID [1-8] and targets [0-180]")
        print("[+] Type 'exit' to terminate utility.\n")
        
        while True:
            user_input = input("Enter (ServoID:Angle) e.g., 1:90 -> ")
            if user_input.lower() == 'exit':
                break
                
            if ":" in user_input:
                try:
                    servo_id, angle = user_input.split(":")
                    # التحقق من منطقية الزوايا والمدى الميكانيكي للمحرك
                    if 0 <= int(angle) <= 180:
                        formatted_command = f"CAL:{servo_id}:{angle}\n"
                        esp32.write(formatted_command.encode('utf-8'))
                        print(f"[->] Payload transmitted: {formatted_command.strip()}")
                    else:
                        print("[!] Error: Out of bounds angle constraints (0-180).")
                except ValueError:
                    print("[!] Invalid parsing format. Use ID:ANGLE syntax.")
            else:
                print("[!] Syntax error. Delimiter ':' missing.")
                
        esp32.close()
        print("[+] Serial link closed safely.")
    except serial.SerialException:
        print(f"[!] Critical Error: Unable to access peripheral port {port}.")

if __name__ == "__main__":
    run_calibration()