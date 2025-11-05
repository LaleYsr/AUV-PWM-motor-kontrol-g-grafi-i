import matplotlib.pyplot as plt

file_name = "src/veriler.txt" 

pwm_load_groups = {} 

def parcala(veri):
    
    veri_temiz = veri.strip() 
    
    
    parcalar = [p.strip() for p in veri_temiz.split('|')]
    

    if len(parcalar) < 4:
         return None

    pwm_str = parcalar[1].split(' ')[0]
    load_str = parcalar[3]


    pwm = int(pwm_str)
    load = float(load_str)
    return pwm, load
    
        

with open(file_name, 'r', encoding='utf-8') as f:
    for satir in f:
        ayristirilmis_veri = parcala(satir)
        
        if ayristirilmis_veri:
            pwm, load = ayristirilmis_veri
            
            
            if pwm not in pwm_load_groups:
                pwm_load_groups[pwm] = []
            
            pwm_load_groups[pwm].append(load)



grafik_pwm_list = []
grafik_load_ortalama_list = []

sorted_pwm_keys = sorted(pwm_load_groups.keys())

for pwm_key in sorted_pwm_keys:
    load_list = pwm_load_groups[pwm_key]
    
    
    ortalama_load = sum(load_list) / len(load_list)
    
    grafik_pwm_list.append(pwm_key)
    grafik_load_ortalama_list.append(ortalama_load)


plt.figure(figsize=(12, 6))


plt.plot(grafik_pwm_list, grafik_load_ortalama_list, 
         marker='o', linestyle='-', color='b')

plt.title('PWM e Gore Ortalama Load Degeri')
plt.xlabel('PWM')
plt.ylabel('Ortalama Load')


plt.xlim(1050, 1950) 


plt.grid(True)
plt.show()

