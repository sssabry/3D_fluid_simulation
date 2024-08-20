import numpy as np
from datetime import datetime
import plotly.graph_objects as go
import plotly.io as pio
import os

def plot_heatmaps(output_dir, data):
    """Plot multiple heatmaps and save them in a single HTML file."""
    num_steps = len(data)
    
    # Create the initial empty figure
    fig = go.Figure()

    # Add the first heatmap trace to initialize
    first_step = next(iter(data))
    first_grid = data[first_step]
    fig.add_trace(go.Heatmap(z=first_grid, colorscale='Viridis', showscale=True))
    
    # Add frames for each step
    frames = [go.Frame(data=[go.Heatmap(z=grid, colorscale='Viridis', showscale=False)], name=str(step)) 
              for step, grid in data.items()]
    fig.frames = frames

    # Add Play and Pause buttons
    fig.update_layout(
        title_text="Density Heatmaps Animation",
        updatemenus=[{
            'buttons': [
                {
                    'args': [None, {'frame': {'duration': 500, 'redraw': True}, 'fromcurrent': True}],
                    'label': 'Play',
                    'method': 'animate',
                },
                {
                    'args': [[None], {'frame': {'duration': 0, 'redraw': True}, 'mode': 'immediate', 'transition': {'duration': 0}}],
                    'label': 'Pause',
                    'method': 'animate',
                },
            ],
            'direction': 'left',
            'pad': {'r': 10, 't': 50},
            'showactive': True,
            'type': 'buttons',
            'x': 0.1,
            'xanchor': 'right',
            'y': 0,
            'yanchor': 'top',
        }],
        sliders=[{
            'active': 0,
            'steps': [{
                'args': [[str(step)], {'frame': {'duration': 300, 'redraw': True}, 'mode': 'immediate', 'transition': {'duration': 0}}],
                'label': str(step),
                'method': 'animate',
            } for step in sorted(data.keys())],
            'transition': {'duration': 300, 'easing': 'cubic-in-out'},
            'currentvalue': {'prefix': 'Step:', 'visible': True, 'xanchor': 'right'},
        }],
        height=600,
        width=800,
    )

    # Create a timestamped filename
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    filename = f'{output_dir}/heatmap_{timestamp}.html'

    # Save the figure to the timestamped filename
    pio.write_html(fig, file=filename, auto_open=False)
    print(f"Animation saved to {filename}")

def read_density_file(filename):
    """Read density data from a file."""
    with open(filename, 'r') as file:
        lines = file.readlines()
    
    data = {}
    step = None
    for line in lines:
        line = line.strip()
        if line.startswith("Step"):
            step = int(line.split()[1])
            data[step] = []
        elif line:
            row = list(map(float, line.split()))
            data[step].append(row)
    return data

def main():
    input_filename = 'density_output.txt'
    data = read_density_file(input_filename)
    output_dir = 'visualizers/heatmaps'
    os.makedirs(output_dir, exist_ok=True)
    plot_heatmaps(output_dir, data)

if __name__ == "__main__":
    main()
