import numpy as np
import matplotlib.pyplot as plt

# Function to compute average values
def compute_average(filename):
    # Load data from text file
    data = np.loadtxt(filename, delimiter=',')

    # Extracting columns
    speed = data[:, 0]
    Cl = data[:, 1]
    Cm = data[:, 2]

    # Compute average values
    avg_speed = np.mean(speed)
    avg_Cl = np.mean(Cl)
    avg_Cm = np.mean(Cm)

    return avg_speed, avg_Cl, avg_Cm

# List of filenames
filenames = ['50.txt', '60.txt', '70.txt']

# Initialize lists to store average values
avg_speed_list = []
avg_Cl_list = []
avg_Cm_list = []

# Compute average values for each file
for filename in filenames:
    avg_speed, avg_Cl, avg_Cm = compute_average(filename)
    avg_speed_list.append(avg_speed)
    avg_Cl_list.append(avg_Cl)
    avg_Cm_list.append(avg_Cm)

# Define the known points for Cl
x1_cl, y1_cl = avg_speed_list[0], avg_Cl_list[0]  # First point for Cl
x2_cl, y2_cl = avg_speed_list[2], avg_Cl_list[2]  # Second point for Cl

# Define the x-value of the third point for Cl
x3_cl = avg_speed_list[1]  # x-value of the third point for Cl

# Perform linear interpolation to find the y-value of the third point for Cl
y3_cl = y1_cl + (x3_cl - x1_cl) * (y2_cl - y1_cl) / (x2_cl - x1_cl)

print("The y-value of the third point for Cl is:", y3_cl)

# Calculate the slope and intercept for Cl
m1_cl, b1_cl = np.polyfit(avg_speed_list, avg_Cl_list, 1)

# Define the best-fit line function for Cl
y_cl = lambda x: m1_cl * x + b1_cl

# Define x values for the best-fit line for Cl
x_cl = np.linspace(min(avg_speed_list), max(avg_speed_list), 100)

# Calculate the residuals as a percentage for Cl
residuals_percentage_cl = [abs(y_cl(x) - y) / y * 100 for x, y in zip(avg_speed_list, avg_Cl_list)]

# Define the known points for Cm
x1_cm, y1_cm = avg_speed_list[0], avg_Cm_list[0]  # First point for Cm
x2_cm, y2_cm = avg_speed_list[2], avg_Cm_list[2]  # Second point for Cm

# Define the x-value of the third point for Cm
x3_cm = avg_speed_list[1]  # x-value of the third point for Cm

# Perform linear interpolation to find the y-value of the third point for Cm
y3_cm = y1_cm + (x3_cm - x1_cm) * (y2_cm - y1_cm) / (x2_cm - x1_cm)

print("The y-value of the third point for Cm is:", y3_cm)

# Calculate the slope and intercept for Cm
m1_cm, b1_cm = np.polyfit(avg_speed_list, avg_Cm_list, 1)

# Define the best-fit line function for Cm
y_cm = lambda x: m1_cm * x + b1_cm

# Define x values for the best-fit line for Cm
x_cm = np.linspace(min(avg_speed_list), max(avg_speed_list), 100)

# Calculate the residuals as a percentage for Cm
residuals_percentage_cm = [abs(y_cm(x) - y) / y * 100 for x, y in zip(avg_speed_list, avg_Cm_list)]

# Plot average speed vs average Cl
plt.figure()
plt.scatter(avg_speed_list, avg_Cl_list, label='Cl')  # Plot data points for Cl
plt.plot(x_cl, y_cl(x_cl), 'r-', label='Trend Line')  # Plot best-fit line for Cl
plt.xlabel('Speed (m/s)')
plt.ylabel('Cl')
plt.title('Cl vs Speed')
plt.grid(True)
plt.legend()
plt.show()

# Plot average speed vs average Cm
plt.figure()
plt.scatter(avg_speed_list, avg_Cm_list, label='Cm')  # Plot data points for Cm
plt.plot(x_cm, y_cm(x_cm), 'r-', label='Trend Line')  # Plot best-fit line for Cm
plt.xlabel('Speed (m/s)')
plt.ylabel('Cm')
plt.title('Cm vs Speed')
plt.grid(True)
plt.legend()

plt.show()

print("Deflection (%) for Cl:", residuals_percentage_cl)
print("Deflection (%) for Cm:", residuals_percentage_cm)
