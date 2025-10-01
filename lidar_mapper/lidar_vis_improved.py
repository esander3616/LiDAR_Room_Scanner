import serial
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Serial port settings
PORT = '/dev/cu.usbmodem1301'
BAUD = 115200

UPDATE_EVERY = 20  # Update plot every 20 points

def spherical_to_cartesian(r, theta_deg, phi_deg):
    theta = np.radians(theta_deg)
    phi = np.radians(phi_deg)
    x = r * np.cos(phi) * np.cos(theta)
    y = r * np.cos(phi) * np.sin(theta)
    z = r * np.sin(phi)
    return x, y, z

def main():
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Connected to {PORT} at {BAUD} baud.")

    x_vals, y_vals, z_vals, r_vals = [], [], [], []

    plt.ion()
    fig = plt.figure(figsize=(10, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Set up dark observatory-style plot
    fig.patch.set_facecolor('black')
    ax.set_facecolor('black')
    ax.grid(False)
    ax.w_xaxis.pane.set_color((0, 0, 0, 0))
    ax.w_yaxis.pane.set_color((0, 0, 0, 0))
    ax.w_zaxis.pane.set_color((0, 0, 0, 0))
    ax.tick_params(colors='white')
    ax.set_xlabel('X (m)', color='white')
    ax.set_ylabel('Y (m)', color='white')
    ax.set_zlabel('Z (m)', color='white')
    ax.set_xlim(-3, 3)
    ax.set_ylim(-3, 3)
    ax.set_zlim(0, 3)
    ax.set_title('3D Room Mapping Observatory View', color='white')

    # initilized scatter
    scatter = ax.scatter([], [], [], c=[], cmap='plasma', s=5, alpha=0.8)
    colorbar = fig.colorbar(scatter, ax=ax, label='Distance (m)')
    colorbar.ax.yaxis.set_tick_params(color='white')
    colorbar.ax.yaxis.label.set_color('white')
    colorbar.outline.set_edgecolor('white')

    counter = 0

    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line.startswith("r:"):
                try:
                    parts = line.split(',')
                    r = float(parts[0].split(':')[1])
                    theta = float(parts[2].split(':')[1])
                    phi = float(parts[1].split(':')[1])
                    x, y, z = spherical_to_cartesian(r, theta, phi)

                    x_vals.append(x)
                    y_vals.append(y)
                    z_vals.append(z)
                    r_vals.append(r)

                    counter += 1
                    if counter % UPDATE_EVERY == 0:
                        scatter._offsets3d = (x_vals, y_vals, z_vals)
                        scatter.set_array(np.array(r_vals))
                        plt.pause(0.001)

                except Exception as e:
                    print("Parse error:", e)

    except KeyboardInterrupt:
        print("Stopping.")
        ser.close()
        plt.ioff()
        
        # Final full plot with ALL points
        fig = plt.figure(figsize=(10, 8))
        ax = fig.add_subplot(111, projection='3d')
        fig.patch.set_facecolor('black')
        ax.set_facecolor('black')
        ax.grid(False)
        ax.w_xaxis.pane.set_color((0, 0, 0, 0))
        ax.w_yaxis.pane.set_color((0, 0, 0, 0))
        ax.w_zaxis.pane.set_color((0, 0, 0, 0))
        ax.tick_params(colors='white')
        ax.set_xlabel('X (m)', color='white')
        ax.set_ylabel('Y (m)', color='white')
        ax.set_zlabel('Z (m)', color='white')
        ax.set_xlim(-3, 3)
        ax.set_ylim(-3, 3)
        ax.set_zlim(0, 3)
        ax.set_title('Final 3D Room Map', color='white')

        scatter = ax.scatter(x_vals, y_vals, z_vals, c=r_vals, cmap='plasma', s=5, alpha=0.8)
        colorbar = fig.colorbar(scatter, ax=ax, label='Distance (m)')
        colorbar.ax.yaxis.set_tick_params(color='white')
        colorbar.ax.yaxis.label.set_color('white')
        colorbar.outline.set_edgecolor('white')

        plt.show()

if __name__ == "__main__":
    main()