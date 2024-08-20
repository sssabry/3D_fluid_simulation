import numpy as np

def read_density_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    data = []
    step_data = []
    for line in lines:
        line = line.strip()
        if line.startswith("Step"):
            if step_data:
                data.append(np.array(step_data))
                step_data = []
        elif line:
            row = list(map(float, line.split()))
            step_data.append(row)

    if step_data:
        data.append(np.array(step_data))

    return data

def print_ascii(slice_data):
    for row in slice_data:
        for value in row:
            if value > 0.01:
                print('#', end=' ')
            elif value > 0.001:
                print('*', end=' ')
            elif value > 0.0001:
                print('.', end=' ')
            else:
                print(' ', end=' ')
        print()

def main():
    filename = 'density_output.txt'
    data = read_density_file(filename)

    for step_index, step_data in enumerate(data):
        print(f"Step {step_index}")
        print_ascii(step_data)
        print()

if __name__ == "__main__":
    main()
