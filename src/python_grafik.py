import matplotlib.pyplot as plt
import serial
import time


ser = serial.Serial('COM12', 9600, timeout= 0.1)

start_code = input("write start code: ")
pwm = 1500
ser.write(f"{pwm}\n".encode())  

loadcell_verileri = []  
all_loadcell_verileri = []
pwm_verileri = []       
karar = 1
durdur = 0
last_time = time.time()

x = 8 

def change_pwm(pwm, durdur, karar):
    if pwm >= 1900:
        karar = -1
    elif pwm <= 1100:
        karar = 1

    if pwm == 1500:
        durdur += 1

    pwm += x * karar
    return pwm, durdur, karar


while start_code == "start":
    current_time = time.time()
    
    
    if current_time - last_time >= 2:
        pwm_verileri.append(pwm)
        ser.write(f"{pwm}\n".encode())

    
        
        last_time = time.time()
        pwm, durdur, karar = change_pwm(pwm, durdur, karar)
        if durdur == 3:
            break
    
    while len(loadcell_verileri) < 20:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line:  
                loadcell_verileri.append(line)
        
    all_loadcell_verileri.append(loadcell_verileri.copy())
    print(pwm,"\n")
    print(loadcell_verileri,"\n")
    loadcell_verileri.clear()

         
grafik_load_ortalama_list = []
grafik_pwm_list = sorted(set(pwm_verileri))  

for pwm in grafik_pwm_list:
    
    load_vals_for_pwm = []
    for i, pwm_val in enumerate(pwm_verileri):
        if pwm_val == pwm:
            load_vals_for_pwm.extend([float(x) for x in all_loadcell_verileri[i]])
    
    
    ortalama = sum(load_vals_for_pwm) / len(load_vals_for_pwm)
    grafik_load_ortalama_list.append(ortalama)


plt.figure(figsize=(12, 6))

plt.plot(grafik_pwm_list, grafik_load_ortalama_list, 
         marker='o', linestyle='-', color='b')

plt.title('PWM e Gore Ortalama Load Degeri')
plt.xlabel('PWM')
plt.ylabel('Ortalama Load')
plt.grid(True)

plt.xlim(1050, 1950)
plt.show()
      

        

























