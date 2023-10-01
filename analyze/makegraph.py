import matplotlib.pyplot as plt
import csv
import os


def extract_and_save_csv_data(filename):
    with open(filename, "r") as file:
        lines = file.readlines()

    # Extract CSV data
    csv_data = [
        line for line in lines if line.replace(".", "", 1).split(",")[0].isdigit()
    ]

    # Generate new filename by replacing extension with .csv
    csv_filename = os.path.splitext(filename)[0] + ".csv"
    with open(csv_filename, "w", newline="") as file:
        file.writelines(csv_data)

    return csv_data, csv_filename


def plot_graphs(filename):
    csv_data, csv_filename = extract_and_save_csv_data(filename)
    data = list(csv.reader(csv_data))

    # Convert string data to float
    data = [[float(value) for value in row] for row in data]

    x = [row[0] for row in data]  # This line should be outside the loop
    y_columns = list(zip(*data))  # Removed [1:] to include all rows

    labels = ["time[s]", "input[v]", "output[rad/s]", "count"]
    for i, y in enumerate(
        y_columns[1:], 1
    ):  # Start with column 1 as y axis (corrected)
        plt.figure()
        plt.plot(x, y, label=labels[i - 1])  # Corrected index for labels
        plt.xlabel(labels[0])
        plt.ylabel(labels[i])  # Corrected index for labels
        plt.title(
            f"Graph of {labels[i-1]} over {labels[0]}"
        )  # Corrected index for labels
        plt.legend()
        plt.savefig(f"graph_column_{i+1}.png")  # Corrected index for file name

    return csv_filename


if __name__ == "__main__":
    csv_filename = plot_graphs("logs\device-monitor-231002-015913.log")
    print(f"CSV data saved as: {csv_filename}")
