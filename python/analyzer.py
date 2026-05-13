import numpy as np
from matplotlib import pyplot as plt
import pandas as pd
import sys

# Get filename over command line argument
if len(sys.argv) != 2:
    print(f"Wait one CLI arguments, but get {len(sys.argv)-1}! Try again")
    sys.exit(1)

filename = sys.argv[1]
print(f"filename: {filename}")

# 1. Get model type
model_type = filename.split("_")[-1]
model_type = model_type.split(".")[0]

# 2. Use pandas to read file
data = df = pd.read_csv(filename, header=None, names=["time", "packets_size"])

print(f"Type of model: {model_type}\n")
print(data)

time = data.iloc[:,0].to_numpy()
packets_size = data.iloc[:,1].to_numpy()

# 3. AVG packet size
avg_packet_size = np.mean(packets_size)
print(f"AVG packet size: {avg_packet_size}")

# 4. AVG bitrate
simulation_time = time[-1]
avg_bitrate = np.sum(packets_size) *8  / simulation_time
print(f"AVG bitrate: {avg_bitrate}bps")

# 5. AVG packet pause
packet_pause = [np.round(time[i] - time[i-1], 8) for i in range(1, len(time))]
avg_packet_pause = np.mean(packet_pause)
print(f"AVG packet pause: {avg_packet_pause}")

# 6. Hist of packet_pause
plt.subplot(1,2,1)
plt.hist(packet_pause, bins=50)
plt.xlabel("pause")
plt.ylabel("Count")
plt.title("Hist of packets pause")
plt.grid()

# 7. Hist of packets size
plt.subplot(1,2,2)
plt.hist(packets_size, bins=50)
plt.xlabel("size")
plt.ylabel("Count")
plt.title("Hist of packets size")
plt.grid()
plt.show()

print("\n\n")